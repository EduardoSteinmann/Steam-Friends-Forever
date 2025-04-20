//
// Created by wlbra on 3/22/2025.
//

#ifndef GAME_H
#define GAME_H
#include <map>
#include <set>
#include <string>
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
        int getId();

    };
    //inline std::vector<auto> all_games;

    void readGameCSV(std::string path);

    inline std::map<int,Game*> allGames;
}


#endif //GAME_H
