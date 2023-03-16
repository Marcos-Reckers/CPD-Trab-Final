#include "classes/patricia.hpp"
#include "classes/game.hpp"
#include "files/import.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

int main()
{
    Trees::Patricia Patricia;
    auto games = cpd::importGames("../DB_Steam.csv");

    std::cout << games.size() << std::endl;
    std::cout << "Inserting..." << std::endl;
    
    for (auto game : games)
    {
        Patricia.Insert(game.getName(), game.getAppid());
    }
    std::cout << Patricia << std::endl;
    std::cout << Patricia.Size() << std::endl;
    return 0;
}