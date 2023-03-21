#pragma once
#include "strings.hpp"
#include "game.hpp"
#include "Trees/patricia.hpp"
#include "hash.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <tuple>
#include <filesystem>

namespace IO
{
    const std::string folder = "./data";
    const std::string DBName = "/Steam.db"; // Path to the database.
    const std::string patExt = ".pat";      // Path to the patricia tree.
    const std::string langExt = ".lang";    // Path to the languages table.
    const std::string genreExt = ".genre";  // Path to the genres table.
    const std::string devExt = ".dev";      // Path to the developers table.
    const std::string pubExt = ".pub";      // Path to the publishers table.
    const std::string tagExt = ".tag";      // Path to the tags table.

    const int urlIndex = 0;         // Index of the url in the csv file.
    const int nameIndex = 1;        // Index of the name in the csv file.
    const int reviewIndex = 2;      // Index of the review in the csv file.
    const int releaseDateIndex = 3; // Index of the release date in the csv file.
    const int developerIndex = 4;   // Index of the developer in the csv file.
    const int publisherIndex = 5;   // Index of the publisher in the csv file.
    const int popularTagsIndex = 6; // Index of the popular tags in the csv file.
    const int gameDetailsIndex = 7; // Index of the game details in the csv file.
    const int languagesIndex = 8;   // Index of the languages in the csv file.
    const int genreIndex = 9;       // Index of the genre in the csv file.
    const int priceIndex = 10;      // Index of the price in the csv file.
    
    std::vector<DB::Game> importGames(const std::string &path, size_t limit = -1);
    int exportGames(const std::string &path, const std::vector<DB::Game> &games);
    DB::Game getGame(const std::string& path, size_t index);
    int appendGame(const std::string& path, DB::Game& game);
    std::vector<DB::Game> loadGames(const std::string& path);
    int ConvertDatabase(const std::string& path, size_t limit = -1);
    bool databaseExists();
}