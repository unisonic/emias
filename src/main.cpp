#include <emias/global_instance.hpp>
#include <iostream>


namespace {
    
    template <NEmias::ERequestField TField>
    void AddRequestField(TgBot::Message::Ptr message) {
        const std::string chatId = std::to_string(message->chat->id);
        const std::string fieldValue = message->text.substr(message->text.find(' ') + 1);
        NEmias::GChatState[chatId][std::to_string(static_cast<unsigned>(TField))] = fieldValue;

        if (NEmias::GChatState[chatId].size() != 8u) {
            return;
        }

        NEmias::GFullState[chatId].push_back(std::move(NEmias::GChatState[chatId]));
        NEmias::GChatState.erase(chatId);

        FileTools::write(NEmias::GFullState.dump(), std::filesystem::current_path() / "full_state.json");
    }

}

int main() {
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
