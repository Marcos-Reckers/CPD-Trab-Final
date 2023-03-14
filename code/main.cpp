#include "game.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

int main()
{
    std::ifstream myfile;
    myfile.open("../steam_games.csv");
    std::string line;
    std::getline(myfile, line);
    std::getline(myfile, line);
    std::getline(myfile, line);

    auto strings = cpd::customSplit(line, ';');
    
    cpd::Game test(strings[0], strings[1], strings[4], strings[5], strings[3], strings[6], strings[2], strings[10]);
    std::cout << test << std::endl;
    myfile.close();
    return 0;
}