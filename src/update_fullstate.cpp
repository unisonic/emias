/** @file */

#include <emias/update_fullstate.hpp>

#include <emias/global_instance.hpp>

#include <string>
#include <vector>

void NEmias::UpdateFullState() {
    std::vector<std::string> emptyChatIds;
    for (const auto& [chatId, requests] : NEmias::GFullState.items()) {
        if (requests.empty()) {
            emptyChatIds.push_back(chatId);
        }
    }
    for (const auto& chatId : emptyChatIds) {
        NEmias::GFullState.erase(chatId);
    }

    FileTools::write(NEmias::GFullState.dump(4), std::filesystem::current_path() / "full_state.json");
}
