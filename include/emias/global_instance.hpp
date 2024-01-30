/** @file */

#ifndef EMIAS_GLOBAL_INSTANCE_HPP
#define EMIAS_GLOBAL_INSTANCE_HPP


#include <tgbot/tgbot.h>

#include <nlohmann/json.hpp>
using TJson = nlohmann::json;

//! Namespace incorporating code related to the project
namespace NEmias {

    //! Enum with field need to be filled for request
    enum ERequestField { 
        FIRST_NAME,
        LAST_NAME,
        SECOND_NAME,
        SPECIALITY_ID,
        BIRTHDATE,
        OMS_NUMBER,
        START_DATE,
        FINISH_DATE
    };

    extern TJson GChatState; /**< State of the chat with users of the bot */

    /**
     * Full state of all saved requests
     * Initialized with the content of a file "full_state.json" right to the executable file
     */
    extern TJson GFullState;

    /**
     * Object for manipulating the Telegram bot
     * Initialized with the bot token retrieved from Telegram server
     */
    extern TgBot::Bot GMainBot;

} // NEmias


#endif // EMIAS_GLOBAL_INSTANCE_HPP
