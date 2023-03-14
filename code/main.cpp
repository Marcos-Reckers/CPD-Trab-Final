#include "classes/game.hpp"
#include "files/import.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

int main()
{
    auto games = cpd::importGames("../DB_Steam.csv");

    int x = 0;
    std::cout << games.size() << std::endl;
    while(x != -1)
    {
        std::cin >> x;
        std::cout << games[x] << std::endl;
    }
    return 0;
}