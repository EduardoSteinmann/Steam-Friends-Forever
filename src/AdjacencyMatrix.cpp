#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyMatrix.hpp"

void AdjacencyMatrix::insert_user(uint64_t user_id)
{
    SteamUser newUser= SteamUser { user_id };
    if(index_graph.find(user_id)==index_graph.end())
    {
        index_graph.insert( std::make_pair(newUser,index_graph.size()+1));
        adj_matrix.emplace_back(std::vector<bool>{});
        adj_matrix[adj_matrix.size()-1].resize(adj_matrix[adj_matrix.size()-1])
        for(int i=0;i<adj_matrix.size();i++)
        {
            
        }

    
    }
    


};
       
   
