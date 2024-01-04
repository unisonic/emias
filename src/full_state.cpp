#include <tgbot/tools/FileTools.h>

#include <nlohmann/json.hpp>
using TJson = nlohmann::json;

namespace NEmias {

    TJson FullState = TJson::parse(FileTools::read(std::filesystem::current_path().string() + "/full_state.json"));

} // NEmias
