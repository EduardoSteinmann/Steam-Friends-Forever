#ifndef _STEAM_USER_H_
#define _STEAM_USER_H_

#include <vector>
#include <string>
#include <cstdint>
#include <utility>
#include <map>

struct SteamUser
{
    uint64_t user_id;
    std::string user_persona;
    std::vector<std::string> top_game_categories;
    std::map<std::string, std::vector<SteamUser>> game_categories;
};

#endif