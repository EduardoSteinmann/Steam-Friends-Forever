#ifndef _STEAM_USER_H_
#define _STEAM_USER_H_

#include <vector>
#include <string>
#include <cstdint>
#include <utility>

constexpr size_t NUMBER_OF_MOST_RECENTLY_PLAYED_GAME_CATEGORIES = 4;

struct SteamUser
{
    uint64_t user_id;
    std::string user_persona;
    std::pair<std::string, uint64_t> game_categories[NUMBER_OF_MOST_RECENTLY_PLAYED_GAME_CATEGORIES];
};

#endif