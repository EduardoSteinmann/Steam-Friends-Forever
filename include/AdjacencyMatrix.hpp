#ifndef _ADJACENCY_MATRIX_H_
#define _ADJACENCY_MATRIX_H_

#include <vector>
#include <cstdint>
#include <unordered_map>
#include "SteamUser.hpp"

class AdjacencyMatrix
{
    public:
    //insert users
    void insert(uint64_t user_id,std::vector<SteamUser> friends);
    void display_user_friends(uint64_t user_id);
    size_t search(size_t user, std::string user_friend);
    int get_insertion_time();
    int get_search_time();
    void display_graph();
    void display_top_games(uint64_t user_id);

    private:
    size_t user_count = 0;
    int insertion_time = 0;
    int search_time = 0;
    std::unordered_map<uint64_t, std::pair<size_t, std::string>> index_graph;
    std::unordered_map<size_t, uint64_t> user_graph;
    std::vector<std::vector<bool>> adj_matrix;
    std::vector<SteamUser> fully_inserted_users;
    size_t insert_user(SteamUser user);
    void insert_user_friends(std::vector<SteamUser>& friends, uint64_t user_id);
};

#endif