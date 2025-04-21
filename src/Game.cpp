
#include "Game.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include "rapidcsv.h"


//
// Created by wlbra on 3/23/2025.
//
namespace Game{
    Game::Game(int i, std::string name,  std::vector<std::string> inputCat)
    {
        this->id=i;
        this->name = name;
        this->categories = inputCat;
    }

    std::string Game::getCategory(int index) {
        return this->categories[index];
    }
    std::vector<std::string> Game::getCategories() {
        return this->categories;
    }

    std::string Game::getName() {
        return this->name;
    }
    int Game::getId() {
        return this->id;
    }

    void readGameCSV(std::string path) {

        std::cout <<"Reading game CSV file"<<std::endl;
        rapidcsv::Document doc(path);
        std::vector<std::string> appIDs = doc.GetColumn<std::string>("AppID");
        std::cout <<"App IDs Read"<<std::endl;

        std::vector<std::string> names = doc.GetColumn<std::string>("Name");
        std::cout <<"Names Read"<<std::endl;

        //std::vector<std::string> categories = doc.GetColumn<std::string>("Categories");
        //std::vector<std::string> genres = doc.GetColumn<std::string>("Genres");
        //I think the library is off somehow, b/c I think tags and genres are swapped
        //std::vector<std::string> tags = doc.GetColumn<std::string>("Tags");
        //They fucked it up and somehow the screenshot column is actually tags of the game
        std::vector<std::string> tags = doc.GetColumn<std::string>("Screenshots");

        std::cout <<"Tags Read"<<std::endl;

        //
        // auto columns= doc.GetColumnNames();
        // for (int i = 0; i < columns.size(); ++i) {
        //     std::cout << columns[i] << std::endl;
        // }
        int totalGames = names.size();

        int interval = totalGames/10;
        for (int i = 0; i < totalGames; ++i) {
            int id = stoi(appIDs[i]);

            //this little for loop sepreates all the unique genres f
            std::string name = names[i];
            std::string gameTags = tags[i];
            std::vector<std::string> categories;
            while (gameTags.length() > 0) {
                std::string eachTag = gameTags.substr(0, gameTags.find(","));
                gameTags = gameTags.substr(gameTags.find(",")+1);
                if (gameTags.find(",") == std::string::npos) {
                    eachTag=gameTags;
                    gameTags="";
                }
                categories.emplace_back(eachTag);
            }

            Game* temp= new Game(id, name, categories);
            std::pair<int, Game*> toInsert= std::make_pair(id,temp);
            allGames.insert(toInsert);
        }
    }
}


