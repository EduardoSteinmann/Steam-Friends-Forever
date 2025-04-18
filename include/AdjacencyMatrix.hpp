#ifndef _ADJACENCY_MATRIX_H_
#define _ADJACENCY_MATRIX_H_

#include <vector>
#include <cstdint>
#include <unordered_map>
#include "SteamUser.hpp"

class AdjacencyMatrix
{
    public:
    void insert(uint64_t user_id,std::vector<SteamUser> friends);
       
    private:
    size_t user_count = 0;
    std::unordered_map<uint64_t, size_t> index_graph;
    std::vector<std::vector<bool>> adj_matrix;
    size_t insert_user(uint64_t user_id);
    void insert_user_friends(std::vector<SteamUser>& friends, uint64_t user_id);
};

#endif