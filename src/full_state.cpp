#include <emias/full_state.hpp>

#include <tgbot/tools/FileTools.h>

#include <filesystem>


TJson& NEmias::GetFullState() {
    static std::string fullStateDir = std::filesystem::current_path().string();
    static TJson fullState = TJson::parse(FileTools::read(fullStateDir + "/full_state.json"));
    return fullState;
}
