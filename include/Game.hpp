//
// Created by wlbra on 3/22/2025.
//

#ifndef GAME_H
#define GAME_H
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace Game {
    class Game {
        int id=-1;
        std::string name;
        std::vector<std::string> categories;
    public:
        Game(int i, std::string name,  std::vector<std::string> inputCat);
        std::string getName();
        std::string getCategory(int index);
        std::vector<std::string> getCategories();
        int getId();

    };

    void readGameCSV(std::string path);

    inline std::string pathToCSV="../assets/games.csv";
    inline std::unordered_map<int,Game*> allGames;
}


#endif //GAME_H
