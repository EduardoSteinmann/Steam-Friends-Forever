
#include "Game.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include "rapidcsv.h"


//
// Created by wlbra on 3/23/2025.
//

Game::Game(int i, std::string name,  std::vector<std::string> inputCat)
  {
  this->id=i;
  this->name = name;
  this->categories = inputCat;

  }

std::string Game::getCategory(int index) {
  return this->categories[index];
}
std::string Game::getName() {
  return this->name;
}
int Game::getId() {
  return this->id;
}

void readGameCSV(std::string path) {

    //TODO multithread this
  rapidcsv::Document doc("../archive/games.csv");
  std::vector<std::string> appIDs = doc.GetColumn<std::string>("AppID");
  std::vector<std::string> names = doc.GetColumn<std::string>("Name");
  //std::vector<std::string> categories = doc.GetColumn<std::string>("Categories");
  //std::vector<std::string> genres = doc.GetColumn<std::string>("Genres");
  //I think the library is off somehow, b/c I think tags and genres are swapped
  std::vector<std::string> tags = doc.GetColumn<std::string>("Tags");



  auto columns= doc.GetColumnNames();
  for (int i = 0; i < columns.size(); ++i) {
    std::cout << columns[i] << std::endl;
  }

  for (int i = 0; i < names.size(); ++i) {
    int id=stoi(appIDs[i]);
    std::string name = names[i];
    std::string gameTags = tags[i];
    std::vector<std::string> categories;
    while (gameTags.length() > 0) {
      //TODO make it so F2P and similar tags do not apply
      std::string eachTag=gameTags.substr(0, gameTags.find(","));
      gameTags=gameTags.substr(gameTags.find(",")+1);
      if (gameTags.find(",")==string::npos) {
        std::string eachTag=gameTags;
        gameTags="";

      }
      categories.emplace_back(gameTags);
    }

    Game* temp= new Game(id,name,categories);
    std::pair<int, Game*> toInsert= std::make_pair(id,temp);
    allGames.insert(toInsert);

  }



}


