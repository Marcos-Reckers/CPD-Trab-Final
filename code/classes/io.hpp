#pragma once
#include "game.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>


namespace IO
{
    std::vector<DB::Game> importGames(const std::string &path, size_t limit = -1);
    int exportGames(const std::string& path, const std::vector<DB::Game>& games);
    DB::Game getGame(const std::string& path, size_t index);
}