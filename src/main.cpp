#include <iostream>
#include <unordered_set>

#include "Steam.hpp"
#include "AdjacencyList.hpp"
#include "AdjacencyMatrix.hpp"
#include "Game.hpp"
#include "TerminalDriver.h"

int main(int argc, char* argv[])
{
    Steam::init();

    terminalDriver();
    return 0;
}