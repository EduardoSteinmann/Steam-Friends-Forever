#ifndef _ADJACENCY_LIST_H_
#define _ADJACENCY_LIST_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "SteamUser.hpp"

class AdjacencyList
{
    private:
    time_t insertion_time = 0;
    time_t search_time = 0;
    //first object in the vector is the user itself
    std::unordered_map<uint64_t , std::vector<SteamUser>> graph;


    public:
    void insert_user(uint64_t user_id, std::vector<SteamUser> friends);
    //insert the friends of the given user into the graph
    void insert_friends(uint64_t user_id);
    //displays friends of given user;
    void display_user_friends(uint64_t user_id);
    //displays the whole graph
    void display_graph();

    //searches for a users friend;
    uint64_t search(uint64_t user_id, std::string friend_id);
    const std::vector<SteamUser>& get_friends(uint64_t user_id);

    int get_insertion_time();

    int get_search_time();

    void number_layers(int layers);
    
};
#endif