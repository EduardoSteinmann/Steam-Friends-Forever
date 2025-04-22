#ifndef _STEAM_H_
#define _STEAM_H_

#include <vector>
#include <cstdint>
#include <string>
#include "SteamUser.hpp"
#include "curl/curl.h"
#include "Game.hpp"

namespace Steam
{
    //the int is the appID, second is the link to the corresponding game.



    CURLcode init();
    void deinit();
    void read_API_key();
    std::vector<SteamUser> get_friends(uint64_t user_id);
    std::string get_username(uint64_t user_id);

    void requestAPI();


    //Returns of a vector of first the appID associated with a game
    //and then amount of hours a user has playeed in that game
    std::vector<std::pair<int,int>> get_users_games(uint64_t user_id);
    //sorts the categories of auser based off their playtime.
    std::vector<std::string> getSortedCategories(uint64_t user_id,int amountOfCategories);
    //this should return the users split up into the categories of game they play the most that matches this user
    std::map<std::string, std::vector<SteamUser>> sortFriendsToCategories(uint64_t user_id, std::vector<std::string>& categories, const std::vector<SteamUser>& friends);

};

#endif