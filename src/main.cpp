/** @file */

#include <emias/process_fullstate.hpp>
#include <emias/update_fullstate.hpp>

#include <emias/global_instance.hpp>
#include <emias/request.hpp>

#include <emias/tools/timer.hpp>

#include <chrono>
#include <iostream>
#include <boost/format.hpp>

namespace {

    /**
     * \brief Show parameters of the request
     * @param chatId Id of the chat with user
     * @param requestId Id of the request in chat with user
     */
    void ShowRequest(const std::string chatId, const std::string requestId) {
        using namespace std::string_literals;

        std::string output = "Запрос <b>"s + requestId + "</b>:\n\n";
        output += "<b>Имя врача:</b> "s + NEmias::GFullState[chatId][requestId]["0"].template get<std::string>() + '\n';
        output += "<b>Фамилия врача:</b> "s + NEmias::GFullState[chatId][requestId]["1"].template get<std::string>() + '\n';
        output += "<b>Отчество врача:</b> "s + NEmias::GFullState[chatId][requestId]["2"].template get<std::string>() + '\n';
        output += "<b>Специальность врача:</b> "s + std::to_string(NEmias::GFullState[chatId][requestId]["3"].template get<TJson::number_unsigned_t>()) + '\n';
        output += "<b>Дата рождения пациента:</b> "s + NEmias::GFullState[chatId][requestId]["4"].template get<std::string>() + '\n';
        output += "<b>Полис ОМС пациента:</b> "s + NEmias::GFullState[chatId][requestId]["5"].template get<std::string>() + '\n';
        output += "<b>Начало приема:</b> "s + NEmias::GFullState[chatId][requestId]["6"].template get<std::string>() + '\n';
        output += "<b>Конец приёма:</b> "s + NEmias::GFullState[chatId][requestId]["7"].template get<std::string>() + '\n';

        NEmias::GMainBot.getApi().sendMessage(chatId, output, false, 0, nullptr, "HTML");
    }

    /**
     * \brief Callback to parameters of all the requests
     * @param message Pointers to instance of message from chat with user
     */
    void ShowFullState(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        for (const auto& [requestId, _] : NEmias::GFullState[chatId].items()) {
            ShowRequest(chatId, requestId);
        }
    }
    
    /**
     * \brief Save request to full state
     * @param chatId Id of the chat with user
     * 
     * Generates random id of the request
     */
    void SaveRequest(const std::string chatId) {
        std::string requestId = std::to_string(
            std::chrono::high_resolution_clock::now().time_since_epoch().count() % 1000'000'000);

        NEmias::GFullState[chatId][requestId] = std::move(NEmias::GChatState[chatId]);
        NEmias::GChatState.erase(chatId);
        FileTools::write(NEmias::GFullState.dump(4), static_cast<const std::string&>(std::filesystem::current_path() / "full_state.json"));

        ShowRequest(chatId, requestId);
    }

    /**
     * \brief Template of the callback to set request field
     * @param message Pointer to instance of the message from the chat with user
     */
    template <NEmias::ERequestField TField>
    void AddRequestField(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string fieldValue = message->text.substr(message->text.find(' ') + 1);
        if (TField == NEmias::SPECIALITY_ID) {
            NEmias::GChatState[chatId][std::to_string(NEmias::SPECIALITY_ID)] = std::stoull(fieldValue);
        } else {
            NEmias::GChatState[chatId][std::to_string(TField)] = fieldValue;
        }

        if (NEmias::GChatState[chatId].size() == 8) {
            SaveRequest(chatId);
        }
    }

    /**
     * \brief Deletes request from full state
     * @param message Pointer to instance of the message from the chat with user
     */
    void DeleteRequest(TgBot::Message::Ptr message) {
        const std::string requestId = message->text.substr(message->text.find(' ') + 1);
        NEmias::GFullState[std::to_string(message->chat->id)].erase(requestId);
        FileTools::write(NEmias::GFullState.dump(4), static_cast<const std::string&>(std::filesystem::current_path() / "full_state.json"));

        NEmias::GMainBot.getApi().sendMessage(message->chat->id, "Удалён запрос " + requestId);
    }

    /**
     * \brief Clears whole full state for the particular chat
     * @param message Pointer to the instance of message from the chat with user
     */
    void DeleteFullState(TgBot::Message::Ptr message) {
        NEmias::GFullState.erase(std::to_string(message->chat->id));
        FileTools::write(NEmias::GFullState.dump(4), static_cast<const std::string&>(std::filesystem::current_path() / "full_state.json"));

        NEmias::GMainBot.getApi().sendMessage(message->chat->id, "Все запросы удалены");
    }

    /**
     * \brief Show doctors' specialities with its' codes
     * @param message Pointer to the instance of message from the chat with user
     *
     * Makes post request to the https://emias.info to retrieve data
     */
    void ShowAvailableDoctors(TgBot::Message::Ptr message) {
        using namespace std::string_literals;
        std::vector<std::string> params = StringTools::split(message->text, ' ');

        const TJson response = NEmias::PostRequest(boost::str(boost::format(
            R"({"id":"1","jsonrpc":"2.0","method":"getSpecialitiesInfo","params":{"omsNumber":"%1%","birthDate":"%2%"}})")
            % std::move(params[1]) % std::move(params[2])));
                
        std::string userInfo = "<code>Врач -> код\n\n";
        for (const auto& entry : response["result"]) {
            if (entry["isMultipleLpuSpeciality"].template get<bool>()) {
                continue;
            }

            userInfo += entry["name"].template get<std::string>() + '\n' + "\t -> " + entry["code"].template get<std::string>() + '\n';
        }
        userInfo += "</code>";

        NEmias::GMainBot.getApi().sendMessage(message->chat->id, userInfo, false, 0, nullptr, "HTML");
    }

    /**
     * \brief Assigns callbacks for listeners of telegram api
     */
    void AssignCallbacks() {
        NEmias::GMainBot.getEvents().onCommand("AvailableDoctors", ShowAvailableDoctors);

        NEmias::GMainBot.getEvents().onCommand("ShowFullState", ShowFullState);

        NEmias::GMainBot.getEvents().onCommand("DeleteRequest", DeleteRequest);
        NEmias::GMainBot.getEvents().onCommand("DeleteFullState", DeleteFullState);

        NEmias::GMainBot.getEvents().onCommand("AddFirstName", AddRequestField<NEmias::FIRST_NAME>);
        NEmias::GMainBot.getEvents().onCommand("AddLastName", AddRequestField<NEmias::LAST_NAME>);
        NEmias::GMainBot.getEvents().onCommand("AddSecondName", AddRequestField<NEmias::SECOND_NAME>);
        NEmias::GMainBot.getEvents().onCommand("AddSpeciality", AddRequestField<NEmias::SPECIALITY_ID>);
        NEmias::GMainBot.getEvents().onCommand("AddBirthdate", AddRequestField<NEmias::BIRTHDATE>);
        NEmias::GMainBot.getEvents().onCommand("AddOmsNumber", AddRequestField<NEmias::OMS_NUMBER>);
        NEmias::GMainBot.getEvents().onCommand("AddStartDate", AddRequestField<NEmias::START_DATE>);
        NEmias::GMainBot.getEvents().onCommand("AddFinishDate", AddRequestField<NEmias::FINISH_DATE>);
    }
}

/**
 * \brief Main function with request loop
 */
int main() {
    AssignCallbacks();

    std::int32_t maxUpdateId = 0;
    try {
        std::vector<TgBot::Update::Ptr> updates = NEmias::GMainBot.getApi().getUpdates(0, 1000, 10, nullptr);
        for (const auto& update : updates) {
            maxUpdateId = std::max(maxUpdateId, update->updateId);
        }
        std::cout << "Unconfirmed updates handled" << '\n';
    } catch (TgBot::TgException& e) {
        std::cout << "[ TgBot exception ]: Couldn't retrieve unconfirmed updates" << '\n' << e.what() << '\n';
    }

    try {
        TgBot::TgLongPoll longPoll(NEmias::GMainBot, maxUpdateId + 1);
        NEmias::NTools::TTimer timer(std::chrono::seconds(300));
        while (true) {
            std::cout << "Long poll started" << '\n';
            longPoll.start();

            if (timer.PeriodElapsed()) {
                std::cout << "Processing full state" << '\n';
                if (NEmias::ProcessFullState()) {
                    NEmias::UpdateFullState();
                }
                timer.Reset();
            }
        }
    } catch (TgBot::TgException& e) {
        std::cout << "[ TgBot exception ]" << '\n' << e.what() << '\n';
    }
    return 0;
}
