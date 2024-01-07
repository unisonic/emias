#include <emias/global_instance.hpp>
#include <iostream>

namespace NEmias {

    enum class RequestField { 
        FIRST_NAME,
        LAST_NAME,
        SECOND_NAME,
        SPECIALITY_ID,
        BIRTHDATE,
        OMS_ID,
        START_DATE,
        FINISH_DATE
    };

} // NEmias

namespace 

namespace {

    void HelpCommand(TgBot::Message::Ptr message) {
        NEmias::GMainBot.getApi().sendMessage(
            message->chat->id,
            R"(
                Бот дожидается заполнения необходимой информации о запросе.
                Как только информация заполнена, запрос принимается в обработку
                и с некоторой переодичностью отправляется на <a href="https://emias.info">emias</a>.

                Список команд:
                <b>/help</b> - вывести это сообщение.
                
            )",
            true,
            0,
            nullptr,
            "HTML"
        );
    }

    void TrySaveRequest(const std::string chatId) {
        if (NEmias::GChatState[chatId].size() != 8u) {
            return;
        }

        NEmias::GFullState[chatId].push_back(std::move(NEmias::GChatState[chatId]));
        NEmias::GChatState.erase(chatId);
    }

    void AddFirstNameCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string firstName = message->text.substr(13);
        
        NEmias::GChatState[chatId]["FirstName"] = firstName;
        TrySaveRequest(chatId);
    }

    void AddLastNameCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string lastName = message->text.substr(12);
        
        NEmias::GChatState[chatId]["LastName"] = lastName;
        TrySaveRequest(chatId);
    }

    void AddSecondNameCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string secondName = message->text.substr(14);
        
        NEmias::GChatState[chatId]["SecondName"] = secondName;
        TrySaveRequest(chatId);
    }

    void AddBirthdateCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string birthdate = message->text.substr(13);
        
        NEmias::GChatState[chatId]["Birthdate"] = birthdate;
        TrySaveRequest(chatId);
    }

    void AddOmsIdCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string omsId = message->text.substr(9);
        
        NEmias::GChatState[chatId]["OmsId"] = omsId;
        TrySaveRequest(chatId);
    }

    void AddStartDateCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string startDate = message->text.substr(13);
        
        NEmias::GChatState[chatId]["StartDate"] = startDate;
        TrySaveRequest(chatId);
    }

    void AddFinishDateCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string finishDate = message->text.substr(14);
        
        NEmias::GChatState[chatId]["FinishDate"] = finishDate;
        TrySaveRequest(chatId);
    }

    void AddSpecialityCommand(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string speciality = message->text.substr(14);
        
        NEmias::GChatState[chatId]["FinishDate"] = finishDate;
        TrySaveRequest(chatId);
    }

} // NEmias


int main() {
    NEmias::GMainBot.getEvents().onCommand("help", NEmias::HelpCommand);
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
