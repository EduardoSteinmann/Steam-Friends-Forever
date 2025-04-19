#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyMatrix.hpp"

void AdjacencyMatrix::insert(uint64_t user_id, std::vector<SteamUser> friends)
{
    //inserting user
    insert_user(user_id);
    //inserting friends of user
    insert_user_friends(friends, user_id);
}

size_t AdjacencyMatrix::insert_user(uint64_t user_id)
{
    if(index_graph.find(user_id) == index_graph.end())
    {
        index_graph[user_id] = adj_matrix.size();
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
    return index_graph[user_id];
}

void AdjacencyMatrix::insert_user_friends(std::vector<SteamUser>& friends , uint64_t user_id)
{
    size_t user_index = index_graph[user_id];
    for(size_t i = 1; i < friends.size(); i++)
    {
        auto& user = friends[i];

        size_t friend_index = insert_user(user.user_id);
        
        adj_matrix[user_index][friend_index] = true;
        adj_matrix[friend_index][user_index] = true;
    }
}