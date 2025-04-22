//
// Created by Kasra on 4/20/2025.
//

#include <iostream>
#include "TerminalDriver.h"
#include "AdjacencyMatrix.hpp"
#include "AdjacencyList.hpp"
#include <algorithm>
#include <sstream>
#include <Steam.hpp>
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLDWHITE   "\033[1m\033[37m"

enum STATE
{
    LS,
    CDOT,
    CD,
    TI,
    TS,
    TG,
    AD,
    LA,
    EXIT,
    HELP,
    INVALID,
    NONE,
    AUTO,
    ADJMATRIX,
    ADJLIST
};

enum FRIEND_GENERATION
{
    AUTO_GENERATED,
    USER_GENERATED,
};

//input to all lower case
std::string to_lower(const std::string& input)
{
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}
//trim extra spaces from front and end of input
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Tokenize input into parts
std::vector<std::string> split_command(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

void load_csv()
{
    std::cout << "\nPlease stand by as all 93182 games are loaded\n";
    Game::readGameCSV(Game::pathToCSV);
}

void print_welcome() {
    std::cout << "====================================\n";
    std::cout << " Welcome to Steam Friends Forever!\n";
    std::cout << " Type 'help' to see available commands.\n";
    std::cout << "====================================\n\n";
}

void print_help()
{
    std::cout << "=====================================";
    std::cout << "\nAvailable Commands:\n";
    std::cout << "  cd <name>     - Change directory to <name>\n";
    std::cout << "  cd..          - Go back to the parent directory\n";
    std::cout << "  ls            - List contents of the current directory\n";
    std::cout << "  la            - Shows how deep in the social network you are\n";
    std::cout << "  ad            - Show all directories recursively\n";
    std::cout << "  ti            - Shows duration of last insertion\n";
    std::cout << "  ts            - Shows duration of last search\n";
    std::cout << "  tg            - Shows top played games and with who\n";
    std::cout << "  auto <number> - Automatically generate and insert <number> of users and their friends\n";
    std::cout << "  exit          - Exit the CLI tool\n";
    std::cout << "  help          - Show this help message\n";
}


//returns a command code if command is known otherwise -1;
STATE command_handler(std::string command)
{
    command = trim(command);
    command = to_lower(command);
    std::vector<std::string> tokens = split_command(command);
    try
    {
        if (tokens.at(0) == "exit")
        {
            exit(0);
        }
        if (tokens.at(0) == "help")
        {
            print_help();
            return STATE::HELP;
        }
        if (tokens.at(0) == "cd")
        {
            try {
                if (tokens.at(1) == "..") {
                    return STATE::CDOT;
                }
            } catch (std::out_of_range& e) {
                return STATE::INVALID;
            }

            return STATE::CD;
        }
        if (tokens.at(0) == "ls")
        {
            return STATE::LS;
        }
        if (tokens.at(0) == "ad")
        {
            return STATE::AD;
        }
        if (tokens.at(0) == "cd..")
        {
            return STATE::CDOT;
        }
        if (tokens.at(0) == "ti")
        {
            return STATE::TI;
        }
        if (tokens.at(0) == "ts")
        {
            return STATE::TS;
        }
        if (tokens.at(0) == "tg")
        {
            return STATE::TG;
        }
        if (tokens.at(0) == "la")
        {
            return STATE::LA;
        }
        if (tokens.at(0) == "auto")
        {
            return STATE::AUTO;
        }
    }
    catch (...)
    {
        return STATE::INVALID;
    }
    return STATE::INVALID;
}

void adjacency_matrix_terminal()
{
    AdjacencyMatrix adjacency_matrix;
    std::vector<uint64_t> hierarchy = {};
    std::string steam_user = "";
    FRIEND_GENERATION generation = FRIEND_GENERATION::USER_GENERATED;
    //while loop for initial person we insert;
    while (steam_user.empty() == true)
    {
        std::cout <<"\n-Enter Steam User ID or Enter auto <number> to Generate Graph With <number> users\n";
        std::cout << ">>";
        std::getline(std::cin , steam_user);
        try
        {
            uint64_t Id = std::stoull(steam_user);
            std::vector<SteamUser> friends = Steam::get_friends(Id);
            adjacency_matrix.insert(Id , friends);
            hierarchy.push_back(Id);
            std::cout << "\n Users Data Inserted\n";
        }
        catch (const std::exception& e)
        {
            STATE handled = command_handler(steam_user);
            if(handled == STATE::AUTO)
            {
                generation = FRIEND_GENERATION::AUTO_GENERATED;
                std::vector<std::string> tokens = split_command(steam_user);
                size_t amount_of_users = std::stoull(tokens[1]);
                std::vector<std::pair<uint64_t, std::vector<SteamUser>>> generated_users = Steam::auto_user_generator(amount_of_users);
                for(const auto& user : generated_users)
                {
                    adjacency_matrix.insert(user.first, user.second);   
                }
                hierarchy.push_back(generated_users[0].first);
                std::cout << "\nSocial Network Auto Generated\n";
            }
            if (handled == STATE::INVALID)
            {
                std::cout << "\nInvalid Input\n" << "Steam User Id's Only Include Numbers\n";
                steam_user = "";
            }
        }
    }
    //while loop for rest of command and app runtime
    std::string command = "";
    while (true)
    {
        std::cout <<">> ";
        std::getline(std::cin , command);
        STATE code = command_handler(command);

        if (code == STATE::INVALID)
        {
            std::cout << "\nInvalid Input\n";
            command = "";
        }
        if (code == STATE::CD)
        {
            //command is cd, get user id after cd and search for it in the current users friends and go to that user
            //if found go to that user
            std::vector<std::string> tokens = split_command(command);
            std::string destination = command.substr(command.find(tokens[0]) + tokens[0].size() + 1);
            std::cout <<"\nDestination User ID: " << destination << "\n";
            uint64_t success = adjacency_matrix.search(hierarchy[hierarchy.size()-1],destination);

            if (success == 0)
            {
                std::cout << "\nFriends not Found\n";
            }
            else
            {
                bool found = false;
                for (size_t i = 0; i < hierarchy.size(); i++)
                {
                    if (success == hierarchy[i])
                    {
                        hierarchy.erase(hierarchy.begin() + i + 1, hierarchy.end());
                        found = true;
                        std::cout << "\nAlready visited\n";
                    }
                }
                if (!found && generation == FRIEND_GENERATION::USER_GENERATED){
                    std::vector<SteamUser> friends = Steam::get_friends(success);
                    adjacency_matrix.insert(success , friends);
                    hierarchy.push_back(success);
                }
                else
                {
                    hierarchy.push_back(success);
                }
            }
        }
        else if (code == STATE::LS)
        {
            //command is ls, display the current users friends
            adjacency_matrix.display_user_friends(hierarchy[hierarchy.size() - 1]);
        }
        else if (code == STATE::AD)
        {
            //command is and display the whole graph
            adjacency_matrix.display_graph();
        }
        else if (code == STATE::CDOT)
        {
            //comand is cd..
            if(hierarchy.size() == 1)
            {
                std::cout << "\nYou are at the root of the social network\n";
            }
            else
            {
                hierarchy.pop_back();
            }
        }
        else if (code == STATE::TI)
        {
            //time of insertion
            std::cout << "\nLast Insertion Duration: " << adjacency_matrix.get_insertion_time() << "ns\n";
        }
        else if (code == STATE::TS)
        {
            //time of search
            std::cout << "\nLast Search Duration: " << adjacency_matrix.get_search_time() << "ns\n";
        }
        else if (code == STATE::TG)
        {
            adjacency_matrix.display_top_games(hierarchy[hierarchy.size() - 1]);
        }
        else if (code == STATE::LA)
        {
            //layer deep
            std::cout <<"\n" << hierarchy.size() << "social circles deep in the social network\n";
        }
    }
}

void adjacency_list_terminal()
{
    AdjacencyList adjacency_list;
    std::vector<uint64_t> hierarchy = {};
    std::string steam_user = "";
    FRIEND_GENERATION generation = FRIEND_GENERATION::USER_GENERATED;
    //while loop for initial person we insert;
    while (steam_user.empty() == true)
    {
        std::cout <<"\n-Enter Steam User ID or Enter auto <number> to Generate Graph With <number> users\n";
        std::cout << ">>";
        std::getline(std::cin , steam_user);
        try
        {
            uint64_t Id = std::stoull(steam_user);
            std::vector<SteamUser> friends = Steam::get_friends(Id);
            adjacency_list.insert_user(Id , friends);
            hierarchy.push_back(Id);
            std::cout << "\n Users Data Inserted\n";
        }
        catch (const std::exception& e)
        {
            STATE handled = command_handler(steam_user);
            if(handled == STATE::AUTO)
            {
                generation = FRIEND_GENERATION::AUTO_GENERATED;
                std::vector<std::string> tokens = split_command(steam_user);
                 size_t amount_of_users = std::stoull(tokens[1]);
                std::vector<std::pair<uint64_t, std::vector<SteamUser>>> generated_users = Steam::auto_user_generator(amount_of_users);
                for(const auto& user : generated_users)
                {
                    adjacency_list.insert_user(user.first, user.second);   
                }
                hierarchy.push_back(generated_users[0].first);
                std::cout << "\nSocial Network Auto Generated\n";
            }
            if (handled == STATE::INVALID)
            {
                std::cout << "\nInvalid Input\n" << "Steam User Id's Only Include Numbers\n";
                steam_user = "";
            }
        }
    }
    //while loop for rest of command and app runtime
    std::string command = "";
    while (true)
    {
        std::cout <<">> ";
        std::getline(std::cin , command);
        STATE code = command_handler(command);
        if (code == STATE::INVALID)
        {
            std::cout << "\nInvalid Input\n";
            command = "";
        }
        if (code == STATE::CD)
        {
            //command is cd, get user id after cd and search for it in the current users friends and go to that user
            //if found go to that user
            std::vector<std::string> tokens = split_command(command);
            std::string destination = command.substr(command.find(tokens[0]) + tokens[0].size() + 1);
            std::cout <<"\nDestination User ID: " << destination << "\n";
            uint64_t success = adjacency_list.search(hierarchy[hierarchy.size()-1],destination);

            if (success == 0)
            {
                std::cout << "\nFriends not Found\n";
            }
            else
            {
                bool found = false;
                for (size_t i = 0; i < hierarchy.size(); i++)
                {
                    if (success == hierarchy[i])
                    {
                        hierarchy.erase(hierarchy.begin() + i + 1, hierarchy.end());
                        found = true;
                        std::cout << "\nAllready visited\n";
                    }
                }
                if (!found && generation == FRIEND_GENERATION::USER_GENERATED){
                    std::vector<SteamUser> friends = Steam::get_friends(success);
                    adjacency_list.insert_user(success , friends);
                    hierarchy.push_back(success);

                }
                else
                {
                    hierarchy.push_back(success);
                }
            }
        }
        else if (code == STATE::LS)
        {
            //command is ls, display the current users friends
            adjacency_list.display_user_friends(hierarchy[hierarchy.size() - 1]);

        }
        else if (code == STATE::AD)
        {
            //command is ad display the whole graph
            adjacency_list.display_graph();

        }
        else if (code == STATE::CDOT)
        {
            if(hierarchy.size() == 1)
            {
                std::cout << "\nYou are at the root of the social network\n";
                continue;
            }
            else{
                hierarchy.pop_back();
            }
        }
        else if (code == STATE::TI)
        {
            //time of insertion
            std::cout << "\nLast Insertion Duration: " << adjacency_list.get_insertion_time() << "ns\n";
        }
        else if (code == STATE::TS)
        {
            //time of search
            std::cout << "\nLast Search Duration: " << adjacency_list.get_search_time() << "ns\n";
        }
        else if (code == STATE::LA)
        {
            //layer deep
            std::cout <<"\n" << hierarchy.size() << "social circles deep in the social network\n";
        }
        else if (code == STATE::TG)
        {
            adjacency_list.display_top_games(hierarchy[hierarchy.size() - 1]);
        }
    }
}

STATE get_data_structure()
{
    std::string choice;
    std::cout << "\n-Choose a data structure to work with\n";
    std::cout << "1.AdjacencyList\n";
    std::cout << "2.AdjacencyMatrix\n";
    std::cout << ">>";
    std::getline(std::cin, choice);
    try
    {
        int num = std::stoi(choice);
        if (num == 1)
        {
            return STATE::ADJLIST;
        }
        if (num == 2)
        {
            return STATE::ADJMATRIX;
        }
        return STATE::NONE;
    }
    catch (...)
    {
        choice = to_lower(choice);

        if (choice == "adjacencylist")
        {
            return STATE::ADJLIST;
        }
        if (choice == "adjacencymatrix")
        {
            return STATE::ADJMATRIX;
        }
        if (choice == "exit")
        {
            return STATE::EXIT;
        }
        if (choice == "help")
        {
            return STATE::HELP;
        }
        std::cout << "\nInvalid Command\n";
        return STATE::INVALID;
    }
}

int terminalDriver()
{
    print_welcome();

    Steam::init();
    load_csv();
    STATE data_structure = get_data_structure();
    while (data_structure == STATE::INVALID || data_structure == STATE::HELP || data_structure == STATE::NONE)
    {
        print_help();
        data_structure = get_data_structure();
    }
    if (data_structure == STATE::EXIT)
    {
        exit(0);
    }
    if (data_structure == STATE::ADJLIST)
    {
        adjacency_list_terminal();
    }
    else if (data_structure == STATE::ADJMATRIX)
    {
        adjacency_matrix_terminal();
    }
    return 0;
}