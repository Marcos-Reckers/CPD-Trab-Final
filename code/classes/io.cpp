#include "io.hpp"

namespace IO
{
    void AddVecToTable(const std::string &str, Tables::Hash &Table, int Appid)
    {
        for (auto tag : STR::customSplit(str, ','))
        {
            if (tag[0] == ' ' && tag.size() > 1)
                tag = tag.substr(1, tag.size() - 1);
            Table.Insert(tag, Appid);
        }
    }

    GamesTuple importGames(const std::string &path, size_t limit)
    {
        std::vector<DB::Game> games;      // Vector of games to be returned.
        Trees::Patricia patricia;         // Patricia tree to be returned.
        Tables::Hash languages;           // Hash table of languages.
        Tables::Hash genres;              // Hash table of genres.
        Tables::Hash developers;          // Hash table of developers.
        Tables::Hash publishers;          // Hash table of publishers.
        Tables::Hash popularTags;         // Hash table of popular tags.

        std::ifstream myfile; // File to be read.
        myfile.open(path);    // Open the file.

        if (!myfile.is_open() || !myfile.good())
        {
            std::cout << "Error opening file" << std::endl;
            return std::make_tuple(games, patricia, std::make_tuple(languages, genres, developers, publishers, popularTags));
        }

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

        std::string line;           // Line to be read.
        std::getline(myfile, line); // Skip the first line.
        std::getline(myfile, line); // Skip the second line.

        while (!myfile.eof() && games.size() < limit) // While we haven't reached the end of the file.
        {
            std::getline(myfile, line);                 // Read the next line.
            auto strings = STR::customSplit(line, ';'); // Split the line into a vector of strings.

            // Add the tags to the game.
            const std::string tags = strings[popularTagsIndex] + ", " + strings[gameDetailsIndex] + ", " + strings[languagesIndex] + ", " + strings[genreIndex];

            DB::Game game(strings[urlIndex], strings[nameIndex], strings[developerIndex],
                          strings[publisherIndex], strings[releaseDateIndex], tags,
                          strings[priceIndex], strings[reviewIndex]);

            patricia.Insert(strings[nameIndex], game.getAppid());

            AddVecToTable(strings[popularTagsIndex], popularTags, game.getAppid());
            AddVecToTable(strings[languagesIndex], languages, game.getAppid());
            AddVecToTable(strings[genreIndex], genres, game.getAppid());
            AddVecToTable(strings[developerIndex], developers, game.getAppid());
            AddVecToTable(strings[publisherIndex], publishers, game.getAppid());

            // Add the game to the vector.
            games.push_back(game); // Add the game to the vector.
        }

        myfile.close();

        auto Hashes = std::make_tuple(languages, genres, developers, publishers, popularTags);
        return std::make_tuple(games, patricia, Hashes);
    }

    int exportGames(const std::string &path, const GamesTuple &games)
    {
        std::ofstream file(path, std::ios::binary); // File to be written to.
        size_t Filesize = 0;                        // Size of the file.

        if (!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return -1;
        }

        if (!file.good())
        {
            std::cout << "Error writing to file" << std::endl;
            return -1;
        }

        for (auto game : std::get<0>(games))
        {
            Filesize += game.writeToFile(file); // Write the game to the file.
        }

        file.close();

        // auto &patricia = std::get<1>(games);
        // std::ofstream patriciaFile(path + ".patricia", std::ios::binary);
        // patriciaFile << patricia << std::endl;
        // patriciaFile.close();

        auto &Hashes = std::get<2>(games);
        std::ofstream langFile(path + ".lang", std::ios::binary);
        for (auto lang : std::get<0>(Hashes).hashTable)
        {
            langFile << lang.first << std::endl;
        }
        langFile.close();

        std::ofstream genreFile(path + ".genre", std::ios::binary);
        for(auto genre : std::get<1>(Hashes).hashTable)
        {
            genreFile << genre.first << std::endl;
        }
        genreFile.close();

        std::ofstream devFile(path + ".dev", std::ios::binary);
        for(auto dev : std::get<2>(Hashes).hashTable)
        {
            devFile << dev.first << std::endl;
        }
        devFile.close();

        std::ofstream pubFile(path + ".pub", std::ios::binary);
        for(auto pub : std::get<3>(Hashes).hashTable)
        {
            pubFile << pub.first << std::endl;
        }
        pubFile.close();

        std::ofstream tagFile(path + ".tag", std::ios::binary);
        for(auto tag : std::get<4>(Hashes).hashTable)
        {
            tagFile << tag.first << std::endl;
        }
        tagFile.close();

        // patricia.writeToFile(path + ".patricia"); // Write the patricia tree to the file.
        // bplus.writeToFile(path + ".bplus");       // Write the bplus tree to the file.
        return Filesize;
    }

    DB::Game getGame(const std::string &path, size_t index)
    {
        std::ifstream file(path, std::ios::binary); // File to be read.

        if (!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return DB::Game();
        }

        if (!file.good())
        {
            std::cout << "Error reading from file" << std::endl;
            return DB::Game();
        }

        DB::Game game; // Game to be returned.

        return game.readFromFile(file, index) ? game : DB::Game(); // Read the game from the file.
    }

    int appendGame(const std::string &path, DB::Game &game)
    {
        std::ofstream file(path, std::ios::binary | std::ios::app); // File to be written to.

        if (!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return 0;
        }

        if (!file.good())
        {
            std::cout << "Error writing to file" << std::endl;
            return 0;
        }

        auto size = game.writeToFile(file); // Write the game to the file.

        file.close();

        std::ifstream file2(path, std::ifstream::ate | std::ifstream::binary);

        if (!file2.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return 0;
        }

        if (!file2.good())
        {
            std::cout << "Error writing to file" << std::endl;
            return 0;
        }

        return file2.tellg() / size;
    }

    std::vector<DB::Game> loadGames(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary); // File to be read.

        if (!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return std::vector<DB::Game>();
        }

        if (!file.good())
        {
            std::cout << "Error reading from file" << std::endl;
            return std::vector<DB::Game>();
        }

        std::vector<DB::Game> games; // Game to be returned.

        for (size_t i = 0; !file.eof(); i++)
        {
            DB::Game game; // Game to be returned.

            game.readFromFile(file, i); // Read the game from the file.
            games.push_back(game);
        }

        return games; // Read the game from the file.
    }
}