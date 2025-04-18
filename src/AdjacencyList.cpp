#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyList.hpp"
#include "Steam.hpp"
#include "SteamUser.hpp"

void AdjacencyList::insert_user(uint64_t user_id, std::vector<SteamUser> friends)
{
    if(this->graph.find(user_id) == this->graph.end())
    {
        this->graph.insert(std::pair<uint64_t, std::vector<SteamUser>>(user_id, friends));
    }
    else
    {
        this->graph[user_id] = friends;
    }
}

void AdjacencyList::insert_friends(uint64_t user_id)
{
    std::vector<SteamUser> friends = Steam::get_friends(user_id);
    this->graph.insert(std::pair<uint64_t, std::vector<SteamUser>>(user_id, friends));
};

const std::vector<SteamUser>& AdjacencyList::get_friends(uint64_t user_id)
{
    return this->graph[user_id];
};
void number_layers(int layers){};