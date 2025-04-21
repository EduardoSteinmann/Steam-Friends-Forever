#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyList.hpp"

#include <iostream>
#include <ostream>

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
    if(this->graph.find(user_id) == this->graph.end())
    {
        std::vector<SteamUser> friends = Steam::get_friends(user_id);
        this->graph.insert(std::pair<uint64_t, std::vector<SteamUser>>(user_id, friends));
    }
};

const std::vector<SteamUser>& AdjacencyList::get_friends(uint64_t user_id)
{
    return this->graph[user_id];
};

void AdjacencyList::display_user_friends(uint64_t user_id)
{
    if (graph.find(user_id) == graph.end())
    {
        std::cout << "User " << user_id << " does not exist" << std::endl;
        return;
    }
    std::vector<SteamUser> friends = graph[user_id];
    std::cout << "-" << friends[0].user_persona << std::endl;
    for (int i = 1; i < friends.size(); i++)
    {
        std::cout << "\t|" << "--"  << friends[i].user_persona << std::endl;
    }
}

uint64_t AdjacencyList::search(uint64_t user_id, std::string friend_id)
{
    if (graph.find(user_id) == graph.end())
    {
        return 0;
    }
    std::vector<SteamUser> friends = graph[user_id];
    for (int i = 0; i < friends.size(); i++)
    {
        if (friends[i].user_persona == friend_id)
        {
            return friends[i].user_id;
        }
    }
    return 0;
}

void AdjacencyList::display_graph()
{
    for (auto element : graph)
    {
        display_user_friends(element.first);
    }
}


void number_layers(int layers){};