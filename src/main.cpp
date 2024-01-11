#include <emias/global_instance.hpp>
#include <iostream>
#include <chrono>

namespace {
    
    void SaveRequest(std::string chatId) {
        std::string requestId = std::to_string(
            std::chrono::high_resolution_clock::now().time_since_epoch().count() % 1000'000'000);

        NEmias::GFullState[chatId][requestId] = std::move(NEmias::GChatState[chatId]);
        NEmias::GChatState.erase(chatId);

        FileTools::write(NEmias::GFullState.dump(4), std::filesystem::current_path() / "full_state.json");
    }

    template <NEmias::ERequestField TField>
    void AddRequestField(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string fieldValue = message->text.substr(message->text.find(' ') + 1);
        NEmias::GChatState[chatId][std::to_string(static_cast<unsigned>(TField))] = fieldValue;

        if (NEmias::GChatState[chatId].size() == 8) {
            SaveRequest(chatId);
        }
    }

    void DeleteFullState(TgBot::Message::Ptr message) {
        NEmias::GFullState.erase(std::to_string(message->chat->id));
        NEmias::GMainBot.getApi().sendMessage(message->chat->id, "Все запросы удалены");

        FileTools::write(NEmias::GFullState.dump(4), std::filesystem::current_path() / "full_state.json");
    }

    void DeleteRequest(TgBot::Message::Ptr message) {
        const std::string requestId = message->text.substr(message->text.find(' ') + 1);
        NEmias::GFullState[std::to_string(message->chat->id)].erase(requestId);
        NEmias::GMainBot.getApi().sendMessage(message->chat->id, "Удалён запрос " + requestId);

        FileTools::write(NEmias::GFullState.dump(4), std::filesystem::current_path() / "full_state.json");
    }

    void ShowAvailableDoctors(TgBot::Message::Ptr message) {
        const std::vector<std::string> params = StringTools::split(message->text, ' ');

        TJson request = TJson::parse(R"({"id": "1","jsonrpc":"2.0","method":"getSpecialitiesInfo"})");
        request["params"]["omsNumber"] = params[1];
        request["params"]["birthDate"] = params[2];

        NEmias::GMainBot.getApi().sendMessage(message->chat->id, request.dump(4));
    }
}

int main() {
    NEmias::GMainBot.getEvents().onCommand("AvailableDoctors", ShowAvailableDoctors);
    NEmias::GMainBot.getEvents().onCommand("DeleteRequest", DeleteRequest);
    NEmias::GMainBot.getEvents().onCommand("DeleteFullState", DeleteFullState);

    NEmias::GMainBot.getEvents().onCommand("AddFirstName", AddRequestField<NEmias::ERequestField::FIRST_NAME>);
    NEmias::GMainBot.getEvents().onCommand("AddLastName", AddRequestField<NEmias::ERequestField::LAST_NAME>);
    NEmias::GMainBot.getEvents().onCommand("AddSecondName", AddRequestField<NEmias::ERequestField::SECOND_NAME>);
    NEmias::GMainBot.getEvents().onCommand("AddSpeciality", AddRequestField<NEmias::ERequestField::SPECIALITY_ID>);
    NEmias::GMainBot.getEvents().onCommand("AddBirthdate", AddRequestField<NEmias::ERequestField::BIRTHDATE>);
    NEmias::GMainBot.getEvents().onCommand("AddOmsId", AddRequestField<NEmias::ERequestField::OMS_ID>);
    NEmias::GMainBot.getEvents().onCommand("AddStartDate", AddRequestField<NEmias::ERequestField::START_DATE>);
    NEmias::GMainBot.getEvents().onCommand("AddFinishDate", AddRequestField<NEmias::ERequestField::FINISH_DATE>);

    try {
        printf("Bot username: %s\n", NEmias::GMainBot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(NEmias::GMainBot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}
