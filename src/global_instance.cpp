#include <tgbot/tgbot.h>

#include <nlohmann/json.hpp>
using TJson = nlohmann::json;


namespace NEmias {

    TgBot::Bot MainBot("6955952912:AAFCdUMqL8bnyk1G9L2H3SawdjaQX-qfiH0");
    TJson FullState = TJson::parse(FileTools::read(std::filesystem::current_path().string() + "/full_state.json"));

} // NEmias
