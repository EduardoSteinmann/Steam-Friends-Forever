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
    std::cout << "  cd..         - Go back to the parent directory\n";
    std::cout << "  ls            - List contents of the current directory\n";
    std::cout << "  ad            - Show all directories recursively\n";
    std::cout << "  exit          - Exit the CLI tool\n";
    std::cout << "  help          - Show this help message\n";
}

//returns a command code if command is known otherwise -1;
int command_handler(std::string command)
{
    command = trim(command);
    command = to_lower(command);
    std::vector<std::string> tokens = split_command(command);
    if (tokens[0] == "exit")
    {
        exit(0);
    }
    if (tokens[0] == "help")
    {
        print_help();
        return 0;
    }
    if (tokens[0] == "cd")
    {
        if (tokens[1] == "..")
        {
            return 4;
        }
        return 1;
    }
    if (tokens[0] == "ls")
    {
        return 2;
    }
    if (tokens[0] == "ad")
    {
        return 3;
    }
    if (tokens[0] == "cd..")
    {
        return 4;
    }

    return -1;
}

void adjacency_matrix_terminal()
{
    Steam::init();
    AdjacencyMatrix adjacency_matrix;
    std::vector<uint64_t> heirarchy = {};
    std::string steam_user = "";
    //while loop for initial person we insert;
    while (steam_user.empty() == true)
    {
        std::cout <<"\n  Enter Steam User ID \n";
        std::getline(std::cin , steam_user);
        try
        {
            uint64_t Id = std::stoull(steam_user);
            std::vector<SteamUser> friends = Steam::get_friends(Id);
            adjacency_matrix.insert(Id , friends);
            heirarchy.push_back(Id);
        }
        catch (const std::exception& e)
        {
            int handled = command_handler(steam_user);
            if (handled == -1)
            {
                std::cout << "\nInvalid Input\n" << "Steam User Id's Only Include Numbers\n";
                steam_user = "";
            }
            else
            {
                steam_user = "";
            }

        }
    }
    //while loop for rest of command and app runtime
}

void adjacency_list_terminal()
{
    Steam::init();
    AdjacencyList adjacency_list;
    std::vector<uint64_t> heirarchy = {};
    std::string steam_user = "";
    //while loop for initial person we insert;
    while (steam_user.empty() == true)
    {
        std::cout <<"\n  Enter Steam User ID \n";
        std::getline(std::cin , steam_user);
        try
        {
            uint64_t Id = std::stoull(steam_user);
            std::vector<SteamUser> friends = Steam::get_friends(Id);
            adjacency_list.insert_user(Id , friends);
            heirarchy.push_back(Id);
        }
        catch (const std::exception& e)
        {
            int handled = command_handler(steam_user);
            if (handled == -1)
            {
                std::cout << "\nInvalid Input\n" << "Steam User Id's Only Include Numbers\n";
                steam_user = "";
            }
            else
            {
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
        int code = command_handler(command);
        if (code == -1)
        {
            std::cout << "\nInvalid Input\n";
            command = "";
        }
        if (code == 1)
        {
            //command is cd, get user id after cd and search for it in the current users friends and go to that user
            //if found go to that user
            std::vector<std::string> tokens = split_command(command);
            std::string destination = command.substr(command.find(tokens[0]) + tokens[0].size() + 1);
            std::cout <<"\nDestination User ID: " << destination << "\n";
            uint64_t success = adjacency_list.search(heirarchy[heirarchy.size()-1],destination);
            if (success == 0)
            {
                std::cout << "\nFriends not Found\n";
            }
            else
            {
                std::vector<SteamUser> friends = Steam::get_friends(success);
                adjacency_list.insert_user(success , friends);
                heirarchy.push_back(success);
            }

        }
        else if (code == 2)
        {
            //command is ls, display the current users friends
            adjacency_list.display(heirarchy[heirarchy.size() - 1]);

        }
        else if (code == 3)
        {
            //command is ad display the whole graph

        }
        else if (code == 4)
        {
            heirarchy.pop_back();
        }
    }
}

int get_data_structure()
{
    std::string choice;
    std::cout << "\nChoose a data structure to work with\n";
    std::cout << " 1.AdjacencyList\n";
    std::cout << " 2.AdjacencyMatrix\n";
    std::getline(std::cin, choice);
    try
    {
        int num = std::stoi(choice);
        if (num == 1)
        {
            return 1;
        }
        if (num == 2)
        {
            return 2;
        }
        return 0;
    }
    catch (std::invalid_argument &e)
    {
        choice = to_lower(choice);

        if (choice == "adjacencylist")
        {
            return 1;
        }
        if (choice == "adjacencymatrix")
        {
            return 2;
        }
        if (choice == "exit")
        {
            return -1;
        }
        if (choice == "help")
        {
            return 3;
        }
        std::cout << "\nInvalid Commad\n";
        return -2;
    }
}

int terminalDriver()
{
    print_welcome();
    int data_structure = get_data_structure();
    while (data_structure == -2)
    {
        print_help();
        data_structure = get_data_structure();
    }
    if (data_structure == -1)
    {
        exit(0);
    }
    if (data_structure == 1)
    {
        adjacency_list_terminal();
    }
    else if (data_structure == 2)
    {
        adjacency_matrix_terminal();
    }
    else if (data_structure == 3)
    {
        print_help();
    }
    return 0;
}