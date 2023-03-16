#include "classes/patricia.hpp"
#include "classes/game.hpp"
#include "files/import.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

int main()
{
    // auto games = cpd::importGames("../DB_Steam.csv");

    // int x = 0;
    // std::cout << games.size() << std::endl;
    // while(x != -1)
    // {
    //     std::cout << "Enter a number: ";
    //     std::cin >> x;
    //     if (x < 0 || x >= games.size())
    //     {
    //         std::cout << "Invalid number" << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << games[x] << std::endl;
    //     }
    // }
    Trees::Patricia tree;
    tree.Insert("a", 1);
    tree.Insert("b", 2);
    tree.Insert("c", 3);
    tree.Insert("ab", 4);
    tree.Insert("abc", 5);

    // auto a = tree.Search("a");

    // std::cout << a << std::endl;
    // std::cout << tree.Search("b") << std::endl;
    // std::cout << tree.Search("c") << std::endl;
    // std::cout << tree.Search("ab") << std::endl;
    // std::cout << tree.Search("abc") << std::endl;

    std::cout << tree << std::endl;
    return 0;
}