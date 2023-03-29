#pragma once
#include "strings.hpp"
#include "datastructures/game.hpp"
#include "datastructures/trees/patricia.hpp"
#include "datastructures/tables/hash.hpp"

namespace IO
{
    const std::string folder = "./data";
    const std::string DBName = "/Steam.db";  // Path to the database.
    const std::string patExt = ".pat";       // Path to the patricia tree.
    const std::string langExt = ".lang";     // Path to the languages table.
    const std::string genreExt = ".genre";   // Path to the genres table.
    const std::string devExt = ".dev";       // Path to the developers table.
    const std::string pubExt = ".pub";       // Path to the publishers table.
    const std::string tagExt = ".tag";       // Path to the tags table.
    const std::string dateExt = ".date";     // Path to the release dates table.
    const std::string priceExt = ".price";   // Path to the prices table.
    const std::string reviewExt = ".review"; // Path to the reviews table.
    const std::string appidExt = ".ids";     // Path to the csv file.

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

    int appendGame(const std::string &path, DB::Game &game);
    std::vector<DB::Game> loadGames(const std::string &path);
    int ConvertDatabase(const std::string &path, size_t limit);
    bool databaseExists();
    std::vector<std::string> getKeys(std::ifstream &file);

    std::vector<int> searchFile(std::ifstream &file, const std::string &key);
    std::vector<int> searchFilePrice(std::ifstream &file, int minPrice, int maxPrice);
    std::vector<int> searchFileDec(std::ifstream &file, int Decade);
    std::vector<int> searchNames(std::ifstream &file, std::ifstream &strings, const std::string &key);
}