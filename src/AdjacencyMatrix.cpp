#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyMatrix.hpp"
#include "Steam.hpp"

#include <chrono>
#include <iostream>
#include <ostream>

void AdjacencyMatrix::insert(uint64_t user_id, std::vector<SteamUser> friends)
{
    auto start = std::chrono::high_resolution_clock::now();
    //inserting user
    insert_user(friends[0]);
    this->fully_inserted_users.push_back(friends[0]);
    //inserting friends of user
    insert_user_friends(friends, user_id);
    auto end = std::chrono::high_resolution_clock::now();
    this->insertion_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

size_t AdjacencyMatrix::insert_user(SteamUser user)
{
    if(index_graph.find(user.user_id) == index_graph.end())
    {
        index_graph[user.user_id] = std::pair<size_t, std::string>(adj_matrix.size(),user.user_persona);
        user_graph[adj_matrix.size()] = user.user_id;
        adj_matrix.emplace_back(std::vector<bool>{});

        //puts false for edges of newest vertex with rest of the graph
        for (size_t row = 0; row < index_graph.size(); row++)
        {
            const size_t row_len = adj_matrix[row].size();
            for (size_t col = 0; col < index_graph.size() - row_len; col++)
            {
                adj_matrix[row].emplace_back(false);
            }
        }
    }
    return index_graph[user.user_id].first;
}

void AdjacencyMatrix::insert_user_friends(std::vector<SteamUser>& friends , uint64_t user_id)
{
    size_t user_index = index_graph[user_id].first;
    for(size_t i = 1; i < friends.size(); i++)
    {
        auto& user = friends[i];

        size_t friend_index = insert_user(user);
        
        adj_matrix[user_index][friend_index] = true;
        adj_matrix[friend_index][user_index] = true;
    }
}

void AdjacencyMatrix::display_user_friends(uint64_t user_id)
{
    if(index_graph.find(user_id) == index_graph.end())
    {
        std::cout << "\nUsers data not yet inserted into graph\n" << std::endl;
        return;
    }
    size_t user_index = index_graph[user_id].first;
    std::cout << index_graph[user_id].second << std::endl;
    for (size_t i = 0; i < adj_matrix[user_index].size(); i++)
    {
        if (adj_matrix[user_index][i] == true)
        {
            for (auto element : index_graph)
            {
                if (element.second.first == i)
                {
                    std::cout << "\t|" << "--" << element.second.second << std::endl;
                }
            }
        }
    }
}

size_t AdjacencyMatrix::search(size_t user, std::string user_friend)
{
    auto start = std::chrono::high_resolution_clock::now();
    size_t user_index = index_graph[user].first;
    size_t friend_index = 0;
    size_t friend_id = 0;
    for (auto element : index_graph)
    {
        if (element.second.second == user_friend)
        {
            friend_id = element.first;
            friend_index = element.second.first;
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    this->search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (adj_matrix[user_index][friend_index] == true)
    {
        return friend_id;
    }
    return 0;
}

void AdjacencyMatrix::display_graph()
{
    int MAX_DISPLAY_NUMBER = 50;
    for (auto element : this->fully_inserted_users)
    {
        if (MAX_DISPLAY_NUMBER <= 0)
        {
            return;
        }
        size_t index = index_graph[element.user_id].first;
        std::cout << "-" << index_graph[element.user_id].second << std::endl;
        for (size_t i = 0; i < adj_matrix[index].size(); i++)
        {
            if (adj_matrix[index][i] == true)
            {
                std::cout << "\t|" << "--" << index_graph[user_graph[i]].second << std::endl;
            }
        }
        MAX_DISPLAY_NUMBER--;
    }
}

void AdjacencyMatrix::display_top_games(uint64_t user_id)
{
    SteamUser user;
    std::cout << "-" << index_graph[user_id].second << std::endl;
    for (size_t i = 0; i < fully_inserted_users.size(); i++)
    {
        if (user_id == fully_inserted_users[i].user_id)
        {
            user = fully_inserted_users[i];
            break;
        }
    }
    for (auto element : user.game_categories)
    {
        std::cout << "\t|--" << element.first << std::endl;
        for (auto fren : element.second)
        {
            std::cout << "\t\t|--" << fren.user_persona << std::endl;
        }
    }
}

int AdjacencyMatrix::get_insertion_time()
{
    return this->insertion_time;
}

int AdjacencyMatrix::get_search_time()
{
    return this->search_time;
}