#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyMatrix.hpp"

#include <iostream>
#include <ostream>

void AdjacencyMatrix::insert(uint64_t user_id, std::vector<SteamUser> friends)
{
    //inserting user
    insert_user(friends[0]);
    //inserting friends of user
    insert_user_friends(friends, user_id);
}

size_t AdjacencyMatrix::insert_user(SteamUser user)
{
    if(index_graph.find(user.user_id) == index_graph.end())
    {
        index_graph[user.user_id] = std::pair<size_t, std::string>(adj_matrix.size(),user.user_persona);
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
        std::cout << "User " << user_id << " does not exist" << std::endl;
        return;
    }
    size_t user_index = index_graph[user_id].first;
    std::cout << index_graph[user_id].second << std::endl;
    for (int i = 0; i < adj_matrix[user_index].size(); i++)
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
