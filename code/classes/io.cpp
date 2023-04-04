#include "io.hpp"

namespace IO
{
    std::vector<DB::Game> importGames(const std::string &path, size_t limit)
    {
        std::vector<DB::Game> games; // Vector of games to be returned.

        std::ifstream myfile; // File to be read.
        myfile.open(path);    // Open the file.

        if (!myfile.good())
            return games;

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

            // Add the game to the vector.
            games.push_back(game); // Add the game to the vector.
        }

        myfile.close();

        return games;
    }

    template <typename T>
    bool openAndImport(Tables::Hash<T> &Hash, const std::string &Extension)
    {
        std::ifstream file;
        file.open(folder + DBName + Extension, std::ios::binary);
        if (!file.good())
            return 0;
        Hash.readFromFile(file);
        file.close();
        return 1;
    }

    template <typename T>
    bool openAndExport(Tables::Hash<T> &Hash, const std::string &Extension)
    {
        std::ofstream file;
        file.open(folder + DBName + Extension, std::ios::binary);
        if (!file.good())
            return 0;
        Hash.writeToFile(file);
        file.close();
        return 1;
    }

    std::vector<std::vector<std::string>> appendGame(const std::string &path)
    {
        //! Imports the Data
        std::ifstream file(folder + DBName + patExt, std::ios::binary);
        std::ifstream file2(folder + DBName + patExt + ".str", std::ios::binary);
        Trees::Patricia patricia; // Patricia tree std::ifstream file;
        if (!file.good() || !file2.good())
            return {{"-1"}};
        patricia.readFromFile(file, file2);
        file.close();
        file2.close();

        Tables::Hash<std::string> languages; // Hash table of languages.
        if (!openAndImport(languages, langExt))
            return {{"-1"}};

        Tables::Hash<std::string> genres; // Hash table of genres.
        if (!openAndImport(genres, genreExt))
            return {{"-1"}};

        Tables::Hash<std::string> developers; // Hash table of developers.
        if (!openAndImport(developers, devExt))
            return {{"-1"}};

        Tables::Hash<std::string> publishers; // Hash table of publishers.
        if (!openAndImport(publishers, pubExt))
            return {{"-1"}};

        Tables::Hash<std::string> popularTags; // Hash table of popular tags.
        if (!openAndImport(popularTags, tagExt))
            return {{"-1"}};

        Tables::Hash<std::string> releaseDates; // Hash table of release dates.
        if (!openAndImport(releaseDates, dateExt))
            return {{"-1"}};

        Tables::Hash<std::string> prices; // Hash table of prices.
        if (!openAndImport(prices, priceExt))
            return {{"-1"}};

        Tables::Hash<std::string> reviews; // Hash table of reviews.
        if (!openAndImport(reviews, reviewExt))
            return {{"-1"}};

        Tables::Hash<int> appids; // Hash table of appids.
        if (!openAndImport(appids, appidExt))
            return {{"-1"}};

        std::fstream gamesFile(folder + DBName, std::ios::binary | std::ios::in | std::ios::out); // open the file at the end. (append)
        if (!gamesFile.good())
            return {{"-1"}};
        auto gamesSize = std::filesystem::file_size(folder + DBName) / 1300; // Get the size of the file.

        //! Imports the data from the file
        std::ifstream myfile(path); // File to be read.
        if (!myfile.good())
            return {{"-1"}};

        std::string line;           // Line to be read.
        std::getline(myfile, line); // Skip the first line.
        std::getline(myfile, line); // Skip the second line.

        while (!myfile.eof()) // While we haven't reached the end of the file.
        {
            std::getline(myfile, line);                 // Read the next line.
            auto strings = STR::customSplit(line, ';'); // Split the line into a vector of strings.

            // Add the tags to the game.
            const std::string tags = strings[popularTagsIndex] + ", " + strings[gameDetailsIndex] + ", " + strings[languagesIndex] + ", " + strings[genreIndex];

            DB::Game game(strings[urlIndex], strings[nameIndex], strings[developerIndex],
                          strings[publisherIndex], strings[releaseDateIndex], tags,
                          strings[priceIndex], strings[reviewIndex]);

            auto pos = gamesSize;

            auto place = appids.Search(game.getAppid());
            if (!place.empty())
            {
                const auto &id = game.getAppid();
                patricia.Delete(game.getName());
                popularTags.DeleteGame(id);
                languages.DeleteGame(id);
                genres.DeleteGame(id);
                developers.DeleteGame(id);
                publishers.DeleteGame(id);
                releaseDates.DeleteGame(id);
                prices.DeleteGame(id);
                reviews.DeleteGame(id);
                pos = place[0];
            }

            patricia.Insert(game.getName(), game.getAppid());
            popularTags.Insert(STR::customSplit(strings[popularTagsIndex], ','), game.getAppid());
            languages.Insert(STR::customSplit(strings[languagesIndex], ','), game.getAppid());
            genres.Insert(STR::customSplit(strings[genreIndex], ','), game.getAppid());
            developers.Insert(STR::customSplit(strings[developerIndex], ','), game.getAppid());
            publishers.Insert(STR::customSplit(strings[publisherIndex], ','), game.getAppid());
            releaseDates.Insert(game.getReleaseDate().getYearStr(), game.getAppid());
            prices.Insert(std::to_string(game.getPriceInt()), game.getAppid());
            reviews.Insert(DB::ReviewsToStr(game.getReviews()), game.getAppid());

            gamesFile.seekp(pos * 1300);
            game.writeToFile(gamesFile);

            appids.Insert(game.getAppid(), pos);
            if (place.empty())
                gamesSize++;
        }

        myfile.close();
        gamesFile.close();

        std::ofstream outFile(folder + DBName + patExt, std::ios::binary);
        std::ofstream outFile2(folder + DBName + patExt + ".str", std::ios::binary);
        if (!outFile.good() || !outFile2.good())
            return {{"-1"}};
        patricia.writeToFile(outFile, outFile2);
        outFile.close();
        outFile2.close();

        if (!openAndExport(languages, langExt))
            return {{"-1"}};

        if (!openAndExport(genres, genreExt))
            return {{"-1"}};

        if (!openAndExport(developers, devExt))
            return {{"-1"}};

        if (!openAndExport(publishers, pubExt))
            return {{"-1"}};

        if (!openAndExport(popularTags, tagExt))
            return {{"-1"}};

        if (!openAndExport(releaseDates, dateExt))
            return {{"-1"}};

        if (!openAndExport(prices, priceExt))
            return {{"-1"}};

        if (!openAndExport(reviews, reviewExt))
            return {{"-1"}};

        if (!openAndExport(appids, appidExt))
            return {{"-1"}};

        return {languages.GetKeys(), genres.GetKeys(), developers.GetKeys(), publishers.GetKeys(), popularTags.GetKeys(), releaseDates.GetKeys(), prices.GetKeys()};
    }

    std::vector<std::vector<std::string>> appendGame(const DB::Game &Game,
                                                     const std::string &Developer,
                                                     const std::string &Publisher,
                                                     const std::string &PopularTags,
                                                     const std::string &GameDetails,
                                                     const std::string &Languages,
                                                     const std::string &Genres)
    {
        //! Imports the Data
        std::ifstream file(folder + DBName + patExt, std::ios::binary);
        std::ifstream file2(folder + DBName + patExt + ".str", std::ios::binary);
        Trees::Patricia patricia; // Patricia tree std::ifstream file;
        if (!file.good() || !file2.good())
            return {{"-1"}};
        patricia.readFromFile(file, file2);
        file.close();
        file2.close();

        Tables::Hash<std::string> languages; // Hash table of languages.
        if (!openAndImport(languages, langExt))
            return {{"-1"}};

        Tables::Hash<std::string> genres; // Hash table of genres.
        if (!openAndImport(genres, genreExt))
            return {{"-1"}};

        Tables::Hash<std::string> developers; // Hash table of developers.
        if (!openAndImport(developers, devExt))
            return {{"-1"}};

        Tables::Hash<std::string> publishers; // Hash table of publishers.
        if (!openAndImport(publishers, pubExt))
            return {{"-1"}};

        Tables::Hash<std::string> popularTags; // Hash table of popular tags.
        if (!openAndImport(popularTags, tagExt))
            return {{"-1"}};

        Tables::Hash<std::string> releaseDates; // Hash table of release dates.
        if (!openAndImport(releaseDates, dateExt))
            return {{"-1"}};

        Tables::Hash<std::string> prices; // Hash table of prices.
        if (!openAndImport(prices, priceExt))
            return {{"-1"}};

        Tables::Hash<std::string> reviews; // Hash table of reviews.
        if (!openAndImport(reviews, reviewExt))
            return {{"-1"}};

        Tables::Hash<int> appids; // Hash table of appids.
        if (!openAndImport(appids, appidExt))
            return {{"-1"}};

        std::fstream gamesFile(folder + DBName, std::ios::binary | std::ios::in | std::ios::out); // open the file at the end. (append)
        if (!gamesFile.good())
            return {{"-1"}};
        auto pos = std::filesystem::file_size(folder + DBName) / 1300; // Get the size of the file.

        auto place = appids.Search(Game.getAppid());
        if (!place.empty())
        {
            const auto &id = Game.getAppid();
            patricia.Delete(Game.getName());
            popularTags.DeleteGame(id);
            languages.DeleteGame(id);
            genres.DeleteGame(id);
            developers.DeleteGame(id);
            publishers.DeleteGame(id);
            releaseDates.DeleteGame(id);
            prices.DeleteGame(id);
            reviews.DeleteGame(id);
            pos = place[0];
        }

        patricia.Insert(Game.getName(), Game.getAppid());
        popularTags.Insert(STR::customSplit(PopularTags, ','), Game.getAppid());
        languages.Insert(STR::customSplit(Languages, ','), Game.getAppid());
        genres.Insert(STR::customSplit(Genres, ','), Game.getAppid());
        developers.Insert(STR::customSplit(Developer, ','), Game.getAppid());
        publishers.Insert(STR::customSplit(Publisher, ','), Game.getAppid());
        releaseDates.Insert(Game.getReleaseDate().getYearStr(), Game.getAppid());
        prices.Insert(std::to_string(Game.getPriceInt()), Game.getAppid());
        reviews.Insert(DB::ReviewsToStr(Game.getReviews()), Game.getAppid());

        gamesFile.seekp(pos * 1300);
        Game.writeToFile(gamesFile);
        appids.Insert(Game.getAppid(), pos);
        gamesFile.close();

        std::ofstream outFile(folder + DBName + patExt, std::ios::binary);
        std::ofstream outFile2(folder + DBName + patExt + ".str", std::ios::binary);
        if (!outFile.good() || !outFile2.good())
            return {{"-1"}};
        patricia.writeToFile(outFile, outFile2);
        outFile.close();
        outFile2.close();

        if (!openAndExport(languages, langExt))
            return {{"-1"}};

        if (!openAndExport(genres, genreExt))
            return {{"-1"}};

        if (!openAndExport(developers, devExt))
            return {{"-1"}};

        if (!openAndExport(publishers, pubExt))
            return {{"-1"}};

        if (!openAndExport(popularTags, tagExt))
            return {{"-1"}};

        if (!openAndExport(releaseDates, dateExt))
            return {{"-1"}};

        if (!openAndExport(prices, priceExt))
            return {{"-1"}};

        if (!openAndExport(reviews, reviewExt))
            return {{"-1"}};

        if (!openAndExport(appids, appidExt))
            return {{"-1"}};

        return {languages.GetKeys(), genres.GetKeys(), developers.GetKeys(), publishers.GetKeys(), popularTags.GetKeys(), releaseDates.GetKeys(), prices.GetKeys()};
    }

    std::vector<DB::Game> loadGames(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary); // File to be read.

        if (!file.good())
            return std::vector<DB::Game>();

        std::vector<DB::Game> games; // Game to be returned.

        for (int i = 0; !file.eof(); i++)
        {
            DB::Game game; // Game to be returned.

            game.readFromFile(file, i); // Read the game from the file.
            games.push_back(game);
        }

        return games; // Read the game from the file.
    }

    int ConvertDatabase(const std::string &path, size_t limit)
    {
        //! Imports the Data
        std::vector<DB::Game> games;            // Vector of games
        Trees::Patricia patricia;               // Patricia tree
        Tables::Hash<std::string> languages;    // Hash table of languages.
        Tables::Hash<std::string> genres;       // Hash table of genres.
        Tables::Hash<std::string> developers;   // Hash table of developers.
        Tables::Hash<std::string> publishers;   // Hash table of publishers.
        Tables::Hash<std::string> popularTags;  // Hash table of popular tags.
        Tables::Hash<std::string> releaseDates; // Hash table of release dates.
        Tables::Hash<std::string> prices;       // Hash table of prices.
        Tables::Hash<std::string> reviews;      // Hash table of reviews.
        Tables::Hash<int> appids;               // Hash table of appids.

        //* Open the input file
        std::ifstream input(path); // File to be read.

        if (!input.good())
            return -1;

        std::string line;          // Line to be read.
        std::getline(input, line); // Skip the first line.
        std::getline(input, line); // Skip the second line.

        while (!input.eof() && games.size() < limit) // While we haven't reached the end of the file.
        {
            std::getline(input, line);                  // Read the next line.
            auto strings = STR::customSplit(line, ';'); // Split the line into a vector of strings.

            // Add the tags to the game.
            const std::string tags = strings[popularTagsIndex] + ", " + strings[gameDetailsIndex] + ", " + strings[languagesIndex] + ", " + strings[genreIndex];

            const DB::Game game(strings[urlIndex], strings[nameIndex], strings[developerIndex],
                                strings[publisherIndex], strings[releaseDateIndex], tags,
                                strings[priceIndex], strings[reviewIndex]);

            patricia.Insert(strings[nameIndex], game.getAppid());

            popularTags.Insert(STR::customSplit(strings[popularTagsIndex], ','), game.getAppid());
            languages.Insert(STR::customSplit(strings[languagesIndex], ','), game.getAppid());
            genres.Insert(STR::customSplit(strings[genreIndex], ','), game.getAppid());
            developers.Insert(STR::customSplit(strings[developerIndex], ','), game.getAppid());
            publishers.Insert(STR::customSplit(strings[publisherIndex], ','), game.getAppid());
            releaseDates.Insert(game.getReleaseDate().getYearStr(), game.getAppid());
            prices.Insert(std::to_string(game.getPriceInt()), game.getAppid());
            reviews.Insert(DB::ReviewsToStr(game.getReviews()), game.getAppid());
            appids.Insert(game.getAppid(), games.size());

            // Add the game to the vector.
            games.push_back(game); // Add the game to the vector.
        }

        input.close();

        //! Exports the Databse

        //* Creates the output folder
        std::filesystem::create_directories(folder);
        //* Output file name
        const auto &DBPath = folder + DBName;

        // outputs the games to the file
        std::fstream database(DBPath, std::ios::binary | std::ios::out);
        if (!database.good())
            return -1;
        for (auto game : games)
        {
            game.writeToFile(database);
        }
        database.close();

        // outputs the patricia tree to the file
        std::ofstream patriciaFile(DBPath + patExt, std::ios::binary);
        std::ofstream patriciaFile2(DBPath + patExt + ".str", std::ios::binary);
        if (!patriciaFile.good() || !patriciaFile2.good())
            return -1;
        patricia.writeToFile(patriciaFile, patriciaFile2);
        patriciaFile.close();
        patriciaFile2.close();

        //* outputs the tables to the file
        std::ofstream langFile(DBPath + langExt, std::ios::binary);
        if (!langFile.good())
            return -1;
        languages.writeToFile(langFile);
        langFile.close();

        std::ofstream genreFile(DBPath + genreExt, std::ios::binary);
        if (!genreFile.good())
            return -1;
        genres.writeToFile(genreFile);
        genreFile.close();

        std::ofstream devFile(DBPath + devExt, std::ios::binary);
        if (!devFile.good())
            return -1;
        developers.writeToFile(devFile);
        devFile.close();

        std::ofstream pubFile(DBPath + pubExt, std::ios::binary);
        if (!pubFile.good())
            return -1;
        publishers.writeToFile(pubFile);
        pubFile.close();

        std::ofstream tagFile(DBPath + tagExt, std::ios::binary);
        if (!tagFile.good())
            return -1;
        popularTags.writeToFile(tagFile);
        tagFile.close();

        std::ofstream dateFile(DBPath + dateExt, std::ios::binary);
        if (!dateFile.good())
            return -1;
        releaseDates.writeToFile(dateFile);
        dateFile.close();

        std::ofstream priceFile(DBPath + priceExt, std::ios::binary);
        if (!priceFile.good())
            return -1;
        prices.writeToFile(priceFile);
        priceFile.close();

        std::ofstream reviewFile(DBPath + reviewExt, std::ios::binary);
        if (!reviewFile.good())
            return -1;
        reviews.writeToFile(reviewFile);
        reviewFile.close();

        std::ofstream appidFile(DBPath + appidExt, std::ios::binary);
        if (!appidFile.good())
            return -1;
        appids.writeToFile(appidFile);
        appidFile.close();

        return 0;
    }

    bool databaseExists()
    {
        namespace fs = std::filesystem;
        if (!fs::is_directory(folder))
            return false;

        const auto &DBPath = folder + DBName;
        return (fs::exists(DBPath) && fs::exists(DBPath + dateExt) &&
                fs::exists(DBPath + devExt) && fs::exists(DBPath + genreExt) &&
                fs::exists(DBPath + appidExt) && fs::exists(DBPath + langExt) &&
                fs::exists(DBPath + patExt) && fs::exists(DBPath + patExt + ".str") &&
                fs::exists(DBPath + priceExt) && fs::exists(DBPath + pubExt) &&
                fs::exists(DBPath + reviewExt) && fs::exists(DBPath + tagExt));
    }

    std::vector<std::string> getKeys(std::ifstream &file)
    {
        std::vector<std::string> keys;
        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            auto end = line.find_first_of(';');
            keys.push_back(line.substr(0, end));
        }
        return keys;
    }

    std::vector<int> searchFile(std::ifstream &file, const std::string &key)
    {
        std::vector<int> appids;

        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            auto semi = line.find_first_of(';');

            if (line.substr(0, semi) == key)
            {
                auto ids = STR::customSplit(line.substr(semi + 1), ' ');

                for (auto id : ids)
                {
                    if (id == "\r" || id == "")
                        continue;
                    appids.push_back(std::stoi(id));
                }

                return appids;
            }
        }

        return std::vector<int>();
    }

    std::vector<int> searchFilePrice(std::ifstream &file, int minPrice, int maxPrice)
    {
        std::vector<int> appids;

        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            auto semi = line.find_first_of(';');
            auto priceStr = line.substr(0, semi);
            if (priceStr == "")
                continue;
            auto price = std::stoi(priceStr);

            if (price >= minPrice && price <= maxPrice)
            {
                auto ids = STR::customSplit(line.substr(semi + 1), ' ');

                for (auto id : ids)
                {
                    if (id == "\r" || id == "")
                        continue;
                    appids.push_back(std::stoi(id));
                }
            }
        }

        return appids;
    }

    std::vector<int> searchFileDec(std::ifstream &file, int Decade)
    {
        std::vector<int> appids;

        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            auto semi = line.find_first_of(';');
            auto year = line.substr(0, semi);
            if (year == "TBA" || year == "NaN" || year == "")
                continue;

            auto intYear = std::stoi(year);

            if (intYear >= Decade && intYear < Decade + 10)
            {
                auto ids = STR::customSplit(line.substr(semi + 1), ' ');

                for (auto id : ids)
                {
                    if (id == "\r" || id == "")
                        continue;
                    appids.push_back(std::stoi(id));
                }
            }
        }

        return appids;
    }

    std::vector<int> searchNames(std::ifstream &file, std::ifstream &strings, const std::string &key)
    {
        Trees::Patricia patricia;
        patricia.readFromFile(file, strings);
        auto appids = patricia.SearchPrefix(key);

        return appids;
    }
}