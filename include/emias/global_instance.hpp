#ifndef EMIAS_GLOBAL_INSTANCE_HPP
#define EMIAS_GLOBAL_INSTANCE_HPP


#include <tgbot/tgbot.h>

#include <nlohmann/json.hpp>
using TJson = nlohmann::json;


namespace NEmias {

    extern TgBot::Bot GMainBot;

    extern TJson GChatState;
    extern TJson GFullState;

} // NEmias


#endif // EMIAS_GLOBAL_INSTANCE_HPP
