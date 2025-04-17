#ifndef _STEAM_H_
#define _STEAM_H_

#include <vector>
#include <cstdint>
#include <string>
#include "SteamUser.hpp"
#include "curl/curl.h"

namespace Steam
{
    int init();
    std::string read_API_key();
    std::vector<SteamUser> get_friends(uint64_t user_id);
    std::string get_username(uint64_t user_id);
};

#endif