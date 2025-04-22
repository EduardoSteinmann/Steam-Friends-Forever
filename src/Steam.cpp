#include "Steam.hpp"
#include "SteamUser.hpp"
#include "curl/curl.h"
#include "json.hpp"
#include <vector>
#include <cstdint>
#include <fstream>
#include <random>
#include <map>
#include <queue>
#include <thread>
#include <unordered_map>
#include "Common.h"
#include "Game.hpp"

namespace Steam {
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
        curl_easy_perform(curl_handle);

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

            friends.push_back(SteamUser{ parsed_friend_id, username });
        }
        friends[0].top_game_categories = getSortedCategories(user_id , 1000);
        friends[0].game_categories = sortFriendsToCategories(user_id, friends[0].top_game_categories , friends);
        return friends;
    }
    
    nlohmann::json requestOwnedGames(uint64_t user_id) 
    {
        std::string id = std::to_string(user_id);
        std::string url =  "http://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=" + API_key + "&steamid=" + id;
        nlohmann::json json_response;

        bool completed = false;
        while (!completed) {
            try {
                response.clear();
                curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
                curl_easy_perform(curl_handle);

                json_response = nlohmann::json::parse(response);
                completed=true;
            }
            catch (...) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        return json_response;
    }

    std::vector<std::pair<int,int>> get_users_games(uint64_t user_id) {
        auto json_response = requestOwnedGames(user_id);
        auto jsonGames = json_response["response"]["games"];

        //first val is gameID, second val  is the total hours associate with the game
        std::vector<std::pair<int,int>> gameWhours;
        for (size_t i=0;i<jsonGames.size();i++) {
            auto gameJson=jsonGames[i];

            int gameID=gameJson["appid"].template get<int>();
            int playtime=gameJson["playtime_forever"].template get<int>();
            gameWhours.push_back(std::make_pair(gameID, playtime));
        }

        //sorted so that the highest played games are first. and it does it based of the second value in the opair
        std::sort(gameWhours.begin(), gameWhours.end(),
              [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
                  return a.second > b.second;
              });
        return gameWhours;
    }
    
    //gets the categories with most hours spent in
    std::vector<std::string> getSortedCategories(uint64_t user_id, int amountOfCategories) {
        //Makes certain allGames is initialized
        if (Game::allGames.size() == 0)
        {
            Game::readGameCSV(Game::pathToCSV);
        }

        std::vector<std::pair<int,int>> games_and_hours = get_users_games(user_id);
        std::unordered_map<std::string,int> categoriesTotalMap;

        for (auto game : games_and_hours)
        {
            int id = game.first;

            if (Game::allGames[id] != nullptr)
            {
                std::vector<std::string> thisGameCat = Game::allGames[id]->getCategories();
                for (auto category : thisGameCat)
                {
                    categoriesTotalMap[category]+=game.second;
                }
            }
        }

        std::priority_queue<
            std::pair<std::string, int>,
            std::vector<std::pair<std::string, int>>,
            std::function<bool(const std::pair<std::string, int>&, const std::pair<std::string, int>&)>
        > maxHeap(
            [](const auto& a, const auto& b) {
                return a.second < b.second;  // For a max heap based on the int value
            }
        );

        // Insert all map elements into the priority queue
        for (const auto& [category, count] : categoriesTotalMap) {
            maxHeap.push({category, count});
        }

        std::vector<std::string> top;
        for (int i=0;i<amountOfCategories && maxHeap.size()>0;i++) {

            top.push_back(maxHeap.top().first);
            maxHeap.pop();
        }

        return top;
    }

    std::map<std::string, std::vector<SteamUser>> sortFriendsToCategories(uint64_t user_id, std::vector<std::string>& categories, const std::vector<SteamUser>& friends)
    {
        std::map<std::string, std::vector<SteamUser>> sorted = {};
        for (SteamUser eachFriend : friends)
        {
            //1000 so it gets all the categories for each game b/c there os a max of 1000
            const int max_amount_of_categories = 1000;
            if (eachFriend.top_game_categories.size() == 0)
            {
                eachFriend.top_game_categories = getSortedCategories(eachFriend.user_id, max_amount_of_categories);
            }

            //this goes through all the categories from highest to lowest of a friend
            bool foundCommon = false;
            for (size_t i = 0; i < eachFriend.top_game_categories.size() && !foundCommon; i++) {
                for (size_t e = 0; e < categories.size(); e++) {
                    if (eachFriend.top_game_categories[i] == categories[e]) {
                        foundCommon = true;
                        sorted[categories[e]].push_back(eachFriend);
                    }
                }
            }
        }
        return sorted;
    }

    SteamUser generate_random_user(std::mt19937& generator, std::uniform_int_distribution<uint64_t>& id_distribution, std::uniform_int_distribution<char>& character_distribution)
    {
        constexpr const char* categories[] = 
        {
            "PvP",
            "FPS",
            "Single Player",
            "RPG",
            "Side Scroller"
        };

        constexpr size_t username_length = 15;
        std::string random_username = "";

        for (size_t i = 0; i < username_length; i++)
        {
            const char random_character = character_distribution(generator);
            random_username += random_character;
        }

        uint64_t random_steam_id = id_distribution(generator);

        SteamUser user = SteamUser{ random_steam_id, random_username };

        for (size_t i = 0; i < sizeof(categories) / sizeof(categories[0]); i++)
        {
            user.top_game_categories.emplace_back(categories[i]);
        }

        return user;
    }

    std::vector<std::pair<uint64_t, std::vector<SteamUser>>> auto_user_generator(size_t amount_of_users)
    {
        //what we need:
        //id generator
        //game generator {FPS, RPG, Adventure, VR, etc.}
        //username generator
        //steps:
        //generate user
        //generate friend list
        std::vector<std::pair<uint64_t, std::vector<SteamUser>>> users = {};
        constexpr const char* game_names[] = 
        {
            "Risk of Rain",
            "Marvel Rivals",
            "Dark Souls",
            "GTA V",
            "Mario"
        };
        uint64_t steam_id_lower_range = 70000000000000000;
        uint64_t steam_id_upper_range = 80000009999999999;

        char valid_ascii_lower_bound = '@';
        char valid_ascii_upper_bound = 'Z';
        constexpr size_t friend_amount = 15;

        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<uint64_t> id_distribution(steam_id_lower_range, steam_id_upper_range);
        std::uniform_int_distribution<char> character_distribution(valid_ascii_lower_bound, valid_ascii_upper_bound);
        SteamUser previous;

        SteamUser parent = generate_random_user(generator, id_distribution, character_distribution);
        std::vector<SteamUser> friends = { parent };

        for (size_t i = 0; i < friend_amount; i++)
        {
            SteamUser friend_user = generate_random_user(generator, id_distribution, character_distribution);
            friends.push_back(friend_user);
        }

        for (size_t j = 0; j < sizeof(game_names) / sizeof(game_names[0]); j++)
        {
            parent.game_categories[game_names[j]].push_back(friends[j + 1]);
        }
        friends[0] = parent;
        users.emplace_back( parent.user_id, friends );
        size_t parent_index = 0;
        while (users.size() < amount_of_users)
        {
            for(size_t i = 2; i < users[parent_index].second.size(); i++)
            {
                SteamUser new_parent = users[parent_index].second[i];
                std::vector<SteamUser> new_friends = { new_parent};
                new_friends.push_back(users[parent_index].second[0]);
                for (size_t j = 0; j < friend_amount - 1; j++)
                {
                    SteamUser friend_user = generate_random_user(generator, id_distribution, character_distribution);
                    new_friends.push_back(friend_user);
                }
                for (size_t j = 0; j < sizeof(game_names) / sizeof(game_names[0]); j++)
                {
                    new_parent.game_categories[game_names[j]].push_back(new_friends[j + 1]);
                }
                new_friends[0] = new_parent;
                users.emplace_back(new_parent.user_id, new_friends);
            }
            parent_index++;
        }

        return users;
    }
};