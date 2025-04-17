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
    //first object in the vector is the user itself
    std::unordered_map<uint64_t , std::vector<SteamUser>> graph;
    public:
    void insert_user(uint64_t user_id, std::vector<SteamUser> friends);
    //insert the friends of the given user into the graph
    void insert_friends(uint64_t user_id);

    void number_layers(int layers);
    
};
#endif