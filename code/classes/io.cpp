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

        return int(file2.tellg()) / size;
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
        std::ofstream database(DBPath, std::ios::binary);
        if (!database.good())
            return -1;
        for (auto game : games)
        {
            game.writeToFile(database);
        }
        database.close();

        // outputs the patricia tree to the file
        std::ofstream patriciaFile(DBPath + patExt, std::ios::binary);
        if (!patriciaFile.good())
            return -1;
        patricia.writeToFile(patriciaFile);
        patriciaFile.close();

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
                fs::exists(DBPath + patExt) && fs::exists(DBPath + priceExt) &&
                fs::exists(DBPath + pubExt) && fs::exists(DBPath + reviewExt) &&
                fs::exists(DBPath + tagExt));
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
}