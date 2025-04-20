#include "Steam.hpp"
#include "SteamUser.hpp"
#include "curl/curl.h"
#include "json.hpp"
#include <vector>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include "Common.h"

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
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_response);
        return CURLE_OK;
    }


    std::vector<SteamUser> get_friends(uint64_t user_id)
    {
       std::vector<SteamUser> friends = {};
        std::string ids = std::to_string(user_id);
        std::string url =  "http://api.steampowered.com/ISteamUser/GetFriendList/v0001/?key=" + API_key + "&steamid=" + ids + "&relationship=friend";
        ids.push_back(',');
        response.clear();
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        auto error = curl_easy_perform(curl_handle);

        sff_debug_printf("\nCURL RETURN CODE: %lld\n", error);
        
        friends.push_back(SteamUser{ user_id });

        nlohmann::json json_response = nlohmann::json::parse(response);

        for (auto friend_json : json_response["friendslist"]["friends"])
        {
            auto friend_id = friend_json["steamid"].template get<std::string>();
            ids += friend_id + ',';
        }

        ids.pop_back();

        //Now time to get the usernames of all the friends
        url = "http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=" + API_key + "&steamids=" + ids;

        response.clear();
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl_handle);

        sff_debug_printf("\n----------------\n");
        sff_debug_printf("\nSTEAM API RESPONSE: %s\n", response.c_str());

        json_response = nlohmann::json::parse(response);

        for (size_t i = 0; i < json_response["response"]["players"].size(); i++)
        {
            auto user_json = json_response["response"]["players"][i];
            auto friend_id = user_json["steamid"].template get<std::string>();
            auto username = user_json["personaname"].template get<std::string>();
            uint64_t parsed_friend_id = std::stoull(friend_id);

            //Maybe change? Do map instead?
            if (parsed_friend_id == user_id)
            {
                friends.insert(friends.begin(), SteamUser{ parsed_friend_id, username });
                continue;
            }

            friends.push_back(SteamUser{ parsed_friend_id, username});
        }

        return friends;
    }

    std::string get_username(uint64_t user_id)
    {
        // if(usernames.find(user_id) != usernames.end())
        // {
        //     return usernames[user_id];
        // }
        // std::string ids = std::to_string(user_id);
        // std::string url =  "http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=" + API_key + "&steamid=" + ids;
        // response.clear();
        // curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        // curl_easy_perform(curl_handle);
        // nlohmann::json json_response = nlohmann::json::parse(response);
        // std::string username = json_response["response"]["players"][0]["personaname"];
        // return username;
        return "";
    }
};