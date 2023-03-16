#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include "../classes/game.hpp"


namespace cpd
{
    std::vector<std::string> customSplit(std::string str, char separator);
    std::vector<Game> importGames(std::string path, size_t limit = -1);
}