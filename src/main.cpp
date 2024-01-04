#include <emias/full_state.hpp>
#include <emias/bot_instance.hpp>

#include <iostream>


int main(int argc, char** argv) {
    std::cout << NEmias::GetFullState().dump(4) << '\n';
    return 0;
}
