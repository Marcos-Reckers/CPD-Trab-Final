#pragma once
#include "strings.hpp"
#include "game.hpp"
#include "Trees/patricia.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <tuple>


namespace IO
{
    typedef std::tuple<std::vector<DB::Game>, Trees::Patricia> GamesTuple; // Tuple of Games, Patricia Tree
    GamesTuple importGames(const std::string &path, size_t limit = -1);
    int exportGames(const std::string& path, const GamesTuple& games);
    DB::Game getGame(const std::string& path, size_t index);
    int appendGame(const std::string& path, DB::Game& game);
    std::vector<DB::Game> loadGames(const std::string& path);
}