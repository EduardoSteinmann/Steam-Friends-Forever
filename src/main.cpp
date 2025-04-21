#include <iostream>
#include "Steam.hpp"
#include "AdjacencyList.hpp"
#include "AdjacencyMatrix.hpp"
#include "AppDriver.h"
#include "Game.hpp"
#include "TerminalDriver.h"
int main(int argc, char* argv[])
{
    // Steam::init();
    // uint64_t steam_id = 76561198122653873;
    // std::vector<SteamUser> friends = Steam::get_friends(steam_id);
    // AdjacencyMatrix adj_matrix;
    // adj_matrix.insert(steam_id, friends);
    //
    // //this loads all games into the allGames static variable associated with the games class
    // // std::string path="Game::pathToCSV";
    // // Game::readGameCSV(Game::pathToCSV);
    // auto result=Steam::getSortedCategories(76561198122653873);
    //
    // for (auto elm : result) {
    //     std::cout << elm << std::endl;
    // }
    //
    //
    // Steam::deinit();
    terminalDriver();
    return 0;
}