#include <iostream>
#include "Steam.hpp"

int main()
{
    Steam::init();
    Steam::get_friends(76561199210022989);
    Steam::deinit();
    return 0;
}