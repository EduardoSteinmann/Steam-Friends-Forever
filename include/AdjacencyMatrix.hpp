#ifndef _ADJACENCY_MATRIX_H_
#define _ADJACENCY_MATRIX_H_

#include <vector>
#include <cstdint>
#include <unordered_map>
#include "SteamUser.hpp"

class AdjacencyMatrix
{
    public:
    void insert_user(uint64_t user_id);
       
    private:
    size_t user_count;
    std::unordered_map<SteamUser, size_t> index_graph;
    std::vector<std::vector<bool>> adj_matrix;
};

#endif