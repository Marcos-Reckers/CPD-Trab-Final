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
    // auto games = cpd::importGames("../DB_Steam.csv", 20);

    // std::cout << games.size() << std::endl;
    // std::cout << "Inserting..." << std::endl;

    // for (auto game : games)
    // {
    //     Patricia.Insert(game.getName(), game.getAppid());
    //     std::cout << Patricia << std::endl;
    // }

    Patricia.Insert("Bola", 1);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Bola", 12);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Bor", 2);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Boc", 3);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Bolacha", 4);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Bolo", 5);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Baco", 6);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Ola", 20);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Olaria", 21);
    std::cout << Patricia << std::endl;
    Patricia.Insert("Cola", 30);

    std::cout << Patricia << std::endl;
    std::cout << Patricia.Size() << std::endl;
    return 0;
}