#include "classes/patricia.hpp"
#include "classes/game.hpp"
#include "classes/hash.hpp"
#include "files/import.hpp"
#include <iostream>
#include <vector>

int main()
{
    // Trees::Patricia Patricia;
    Tables::Hash Hash;
    auto games = cpd::importGames("../DB_Steam.csv", 10);
    
    for (auto game : games)
    {
        auto tags = cpd::customSplit(game.getTags(), ',');
        for(auto tag : tags)
        {
            Hash.Insert(tag, game.getAppid());
        }
    }
    std::cout << Hash;
    return 0;
}