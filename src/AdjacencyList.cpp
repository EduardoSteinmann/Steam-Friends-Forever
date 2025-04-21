#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyList.hpp"

#include <chrono>
#include <iostream>
#include <ostream>

#include "Steam.hpp"
#include "SteamUser.hpp"

void AdjacencyList::insert_user(uint64_t user_id, std::vector<SteamUser> friends)
{
    auto start = std::chrono::high_resolution_clock::now();
    if(this->graph.find(user_id) == this->graph.end())
    {
        this->graph.insert(std::pair<uint64_t, std::vector<SteamUser>>(user_id, friends));
    }
    else
    {
        this->graph[user_id] = friends;
    }
    auto end = std::chrono::high_resolution_clock::now();
    this->insertion_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
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

    auto categories = Steam::getSortedCategories(user_id, 4);
    auto friends = this->get_friends(user_id);

    auto categoriesWithFriends = Steam::sortFriendsToCategories(user_id, categories, friends);

    std::cout << "-" << friends[0].user_persona << std::endl;
    for (auto eachCategorty: categoriesWithFriends)
    {
        std::cout << eachCategorty.first << std::endl;
        for (size_t i = 1; i < eachCategorty.second.size(); i++)
        {
            //this gets the set of friend at index i from the category, and the first value of the set should be itself
            SteamUser theFriend=graph[eachCategorty.second[i]][0];
            std::cout << "\t|" << "--"  << theFriend.user_persona << std::endl;
        }
    }
}

uint64_t AdjacencyList::search(uint64_t user_id, std::string friend_id)
{
    auto start = std::chrono::high_resolution_clock::now();
    if (graph.find(user_id) == graph.end())
    {

        auto end = std::chrono::high_resolution_clock::now();
        this->search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return 0;
    }
    std::vector<SteamUser> friends = graph[user_id];
    for (size_t i = 0; i < friends.size(); i++)
    {
        if (friends[i].user_persona == friend_id)
        {
            auto end = std::chrono::high_resolution_clock::now();
            this->search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            return friends[i].user_id;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    this->search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return 0;
}

void AdjacencyList::display_graph()
{
    for (auto element : graph)
    {
        display_user_friends(element.first);
    }
}

int AdjacencyList::get_insertion_time()
{
    return this->insertion_time;
}

int AdjacencyList::get_search_time()
{
    return this->search_time;
}

void number_layers(int layers){};