#include "Steam.hpp"
#include "SteamUser.hpp"
#include "curl/curl.h"
#include <vector>
#include <cstdint>
#include <fstream>

namespace Steam
{
    CURL* curl_handle = NULL;
    std::string response = "";
    std::string API_key = "";

    void deinit()
    {
        curl_global_cleanup();
    }

    void read_API_key()
    {
        std::ifstream file_stream = std::ifstream("../.env");
        std::string line = "";
        std::getline(file_stream, line);
        API_key = line.substr(sizeof("STEAM_API_KEY=") - 1);
    }

    size_t write_response(void* contents, size_t size, size_t nmemb, std::string* output) 
    {
        size_t total_size = size * nmemb;
        output->append((char*)contents, total_size);
        return total_size;
    }

    CURLcode init()
    {
        read_API_key();
        curl_handle = curl_easy_init();
        if (curl_handle == NULL)
        {
            return CURLE_FAILED_INIT;
        }
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_response);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);
        return CURLE_OK;
    }


    std::vector<SteamUser> get_friends(uint64_t user_id)
    {
        std::vector<SteamUser> friends = {};
        std::string url =  "http://api.steampowered.com/ISteamUser/GetFriendList/v0001/?key=" + API_key + "&steamid=" + std::to_string(user_id) + "&relationship=friend";
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        response = curl_easy_perform(curl_handle);\
        
    }

    std::string get_username(uint64_t user_id)
    {
    }
};