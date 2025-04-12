#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyList.hpp"
#include "Steam.hpp"
#include "SteamUser.hpp"

void AdjacencyList::insert_user(uint64_t user_id, std::vector<SteamUser> friends)
{
    this->graph[user_id] = friends;    
}
void AdjacencyList::insert_friends(uint64_t user_id)
{
    std::vector<SteamUser> friends = Steam::get_friends(user_id);
    this->graph.insert(std::pair<uint64_t, std::vector<SteamUser>>(user_id, friends));
};
void number_layers(int layers){};




