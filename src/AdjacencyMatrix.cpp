#include <vector>
#include <cstdint>
#include <unordered_map>
#include "AdjacencyMatrix.hpp"

void AdjacencyMatrix::insert_user(uint64_t user_id,std::vector<SteamUser> friends)
{
    
    SteamUser newUser= SteamUser { user_id };
    //this just checks if user is already existing, if not adds it to the matrix properly and 
    //updates the size of all older rows 
    if(index_graph.find(user_id)==index_graph.end())
    {
        index_graph[user_id]=adj_matrix.size();
        adj_matrix.emplace_back(std::vector<bool>{});
        adj_matrix[adj_matrix.size()-1].resize(adj_matrix[adj_matrix.size()-1].size());
        for(int i=0;i<adj_matrix.size()-1;i++)
        {
            adj_matrix[i].emplace_back(false);
        }
    }
    //makes certain all inputted friends are also apart of the matrix.
    //If not, also adds them
    int friend_id;
    for(auto user:friends)
    {
        friend_id=user.user_id;
        if(index_graph.find(friend_id)==index_graph.end())
        {
            
            index_graph[friend_id]=adj_matrix.size();
            adj_matrix.emplace_back(std::vector<bool>{});
            adj_matrix[adj_matrix.size()-1].resize(adj_matrix[adj_matrix.size()-1].size());
            for(int i=0;i<adj_matrix.size()-1;i++)
            {
                adj_matrix[i].emplace_back(false);
            }
        }
    }
    //then this creates the connections between the two users both ways.
    //As friends on steam are mutual.
    int indexOfUser=index_graph[user_id];
    int indexOfFriend=0;
    for(auto user:friends)
    {
        friend_id=user.user_id;
        indexOfFriend=index_graph[friend_id];
        adj_matrix[indexOfUser][indexOfFriend]=true;
        adj_matrix[indexOfFriend][indexOfUser]=true;
    }

        


    


};
       
   
