#ifndef _STEAM_USER_H_
#define _STEAM_USER_H_

#include <vector>
#include <string>
#include <cstdint>

struct SteamUser
{
    uint64_t user_id;
    std::string user_persona;
};

#endif