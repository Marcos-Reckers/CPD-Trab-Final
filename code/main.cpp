#include "classes/patricia.hpp"
#include "classes/game.hpp"
#include "classes/hash.hpp"
#include "files/import.hpp"
#include "classes/b+.hpp"
#include <iostream>
#include <vector>

int main()
{
    // Trees::Patricia Patricia;
    // Tables::Hash Hash;
    auto games = cpd::importGames("../DB_Steam.csv", 10);
    Trees::BPlus BPlus;
    for(auto game : games)
    {
        
        BPlus.insert(game.getAppid());
    }
    BPlus.display(BPlus.getRoot());

    return 0;
}