#include <emias/full_state.hpp>
#include <emias/bot_instance.hpp>

#include <iostream>


int main(int argc, char** argv) {
    std::cout << NEmias::FullState.dump(4) << '\n';
    std::cout << NEmias::MainBot.getToken() << '\n';
    return 0;
}
