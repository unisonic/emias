#include <emias/bot_instance.hpp>

TgBot::Bot& NEmias::GetBotInstance() {
    static TgBot::Bot bot("6955952912:AAFCdUMqL8bnyk1G9L2H3SawdjaQX-qfiH0");
    return bot;
}

