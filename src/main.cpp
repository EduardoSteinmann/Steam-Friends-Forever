#include <iostream>
#include "Steam.hpp"
#include "AdjacencyList.hpp"
#include "AdjacencyMatrix.hpp"
int main()
{
    Steam::init();
    uint64_t steam_id = 76561199210022989;
    std::vector<SteamUser> friends = Steam::get_friends(steam_id);
    AdjacencyMatrix adj_matrix;
    adj_matrix.insert(steam_id, friends);
    Steam::deinit();
    return 0;
}