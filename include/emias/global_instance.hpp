#ifndef EMIAS_GLOBAL_INSTANCE_HPP
#define EMIAS_GLOBAL_INSTANCE_HPP


#include <tgbot/tgbot.h>

#include <nlohmann/json.hpp>
using TJson = nlohmann::json;


namespace NEmias {

    enum ERequestField { 
        FIRST_NAME,
        LAST_NAME,
        SECOND_NAME,
        SPECIALITY_ID,
        BIRTHDATE,
        OMS_ID,
        START_DATE,
        FINISH_DATE
    };

    extern TJson GChatState;
    extern TJson GFullState;
    extern TgBot::Bot GMainBot;

} // NEmias


#endif // EMIAS_GLOBAL_INSTANCE_HPP
