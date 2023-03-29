#include "database.hpp"

namespace database
{
    // int Append(const std::vector<std::string> &args)
    // {
    //     if (args.size() < 3)
    //     {
    //         return HelpMessage();
    //     }

    //     auto game = DB::Game();
    //     auto index = IO::appendGame(IO::folder + IO::DBName, game);
    //     if (index == 0)
    //     {
    //         std::cout << "Couldn't append game" << std::endl;

    //         return 1;
    //     }

    //     std::cout << "Game Index: " << index << std::endl;
    //     return 0;
    // }

    std::string convert(const std::string &path, int size)
    {
        size_t i = size;
        auto begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto outCode = IO::ConvertDatabase(path, i);
        auto end = std::chrono::high_resolution_clock::now(); // Stop the timer.
        std::stringstream ss;
        if(outCode == 0)
            ss << "Converted database in ";
        else
            ss << "Failed to convert database in ";
        ss << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms";

        return ss.str();
    }

    std::vector<int> searchFile(const std::string &path, const std::vector<std::string> &keys)
    {
        std::vector<int> ids;
        std::ifstream file;
        file.open(path, std::ios::binary);

        if (!file.good())
            return ids;

        for (auto key : keys)
        {
            auto id = IO::searchFile(file, key);
            for (auto i : id)
            {
                ids.push_back(i);
            }
            file.seekg(0);
        }

        file.close();

        return ids;
    }

    std::vector<int> searchFilePrice(const std::string &path, int minPrice, int maxPrice)
    {
        std::vector<int> ids;
        std::ifstream file;
        file.open(path, std::ios::binary);

        if (!file.good())
            return ids;

        if (minPrice > maxPrice)
            std::swap(minPrice, maxPrice);

        if (maxPrice == -1)
            maxPrice = INT_MAX;

        ids = IO::searchFilePrice(file, minPrice, maxPrice);

        file.close();

        return ids;
    }

    std::vector<int> searchFileDec(const std::string &path, int Decade)
    {
        std::vector<int> ids;
        std::ifstream file;
        file.open(path, std::ios::binary);

        if (!file.good())
            return ids;

        ids = IO::searchFileDec(file, Decade);

        file.close();

        return ids;
    }

    std::vector<DB::Game> searchGames(const std::vector<int> &ids)
    {
        std::vector<DB::Game> games;

        Tables::Hash<int> hash;
        std::ifstream file(IO::folder + IO::DBName + IO::appidExt, std::ios::binary);

        if (!file.good())
            return games;

        hash.readFromFile(file);
        file.close();

        std::ifstream gameFile(IO::folder + IO::DBName, std::ios::binary);
        for (auto &i : ids)
        {
            auto index = hash.Search(i);
            if (index.size() == 0)
                continue;

            auto &gameIndex = index[0];
            DB::Game game;
            game.readFromFile(gameFile, gameIndex);
            gameFile.seekg(0);

            games.push_back(game);
        }
        gameFile.close();
        return games;
    }

    std::vector<int> searchNames(const std::string &path, const std::string &name)
    {
        std::ifstream file;
        std::ifstream file2;
        file.open(path, std::ios::binary);
        file2.open(path + ".str", std::ios::binary);

        if (!file.good() || !file2.good())
            return std::vector<int>();

        auto ids = IO::searchNames(file, file2, name);

        file.close();
        file2.close();

        return ids;
    }

    std::vector<int> intersection(const std::vector<int> &a, const std::vector<int> &b, bool type)
    {
        std::unordered_set<int> temp;
        for (auto i : a)
        {
            for (auto j : b)
            {
                if (i == j)
                {
                    temp.insert(i);
                }
                else if (type)
                {
                    temp.insert(i);
                    temp.insert(j);
                }
            }
        }

        return std::vector<int>(temp.begin(), temp.end());
    }

    std::vector<std::vector<std::string>> search(const std::vector<std::string> &genres,
                                                 const std::vector<std::string> &languages,
                                                 const std::vector<std::string> &tags,
                                                 const std::vector<std::string> &dates,
                                                 const std::vector<std::string> &developers,
                                                 const std::vector<std::string> &publishers,
                                                 const std::vector<std::string> &reviews,
                                                 const std::string &name,
                                                 int minPrice, int maxPrice, int decade, int appID, bool searchType)
    {
        if (appID != -1 && searchType == 0)
        {
            auto games = searchGames({appID});
            if (games.size() == 0)
            {
                return {
                    {"No games with that ID found."}};
            }
            return {games[0].DataOut()};
        }

        std::array<std::pair<std::vector<int>, bool>, 12> appids;
        for (auto &i : appids)
            i.second = false;

        if (genres.size() != 0)
            appids[0] = {searchFile(IO::folder + IO::DBName + IO::genreExt, genres), true}; // genres Ids

        if (languages.size() != 0)
            appids[1] = {searchFile(IO::folder + IO::DBName + IO::langExt, languages), true}; // languages Ids

        if (tags.size() != 0)
            appids[2] = {searchFile(IO::folder + IO::DBName + IO::tagExt, tags), true}; // tags Ids

        if (dates.size() != 0)
            appids[3] = {searchFile(IO::folder + IO::DBName + IO::dateExt, dates), true}; // dates Ids

        if (developers.size() != 0)
            appids[4] = {searchFile(IO::folder + IO::DBName + IO::devExt, developers), true}; // developers Ids

        if (publishers.size() != 0)
            appids[5] = {searchFile(IO::folder + IO::DBName + IO::pubExt, publishers), true}; // publishers Ids

        if (minPrice != -1 || maxPrice != -1)
            appids[6] = {searchFilePrice(IO::folder + IO::DBName + IO::priceExt, minPrice, maxPrice), true}; // price Ids

        if (decade != -1)
            appids[7] = {searchFileDec(IO::folder + IO::DBName + IO::dateExt, decade), true}; // decade Ids

        if (reviews.size() != 0)
            appids[8] = {searchFile(IO::folder + IO::DBName + IO::reviewExt, reviews), true}; // reviews Ids

        if(name.length() != 0)
            appids[10] = {searchNames(IO::folder + IO::DBName + IO::patExt, name), true}; // name Ids

        std::vector<int> Ids;
        bool noSearch = true;
        bool first = true;
        for (auto &i : appids)
        {
            if (i.second && first)
            {
                first = false;
                noSearch = false;
                Ids = i.first;
            }

            if (i.second)
            {
                Ids = intersection(Ids, i.first, searchType);
            }
        }

        std::vector<DB::Game> games;
        if (noSearch)
        {
            games = IO::loadGames(IO::folder + IO::DBName);
        }
        else
        {
            games = searchGames(Ids);
            if (games.size() == 0)
            {
                return {{"No games found."}};
            }
        }

        std::vector<std::vector<std::string>> output;
        for (auto &i : games)
            output.push_back(i.DataOut());

        return output;
    }

    bool validate(void)
    {
        return IO::databaseExists();
    }

    std::vector<std::string> getField(const std::string &field)
    {
        std::ifstream file;

        switch (field[0])
        {
        case 'd':
            switch (field[1])
            {
            case 'a':
                // date hash keys
                file.open(IO::folder + IO::DBName + IO::dateExt, std::ios::binary);
                break;
            case 'e':
                // developer hash keys
                file.open(IO::folder + IO::DBName + IO::devExt, std::ios::binary);
                break;
            }
            break;
        case 'g':
            // genre hash keys
            file.open(IO::folder + IO::DBName + IO::genreExt, std::ios::binary);
            break;
        case 'l':
            // language hash keys
            file.open(IO::folder + IO::DBName + IO::langExt, std::ios::binary);
            break;
        case 'p':
            switch (field[1])
            {
            case 'a':
                // file.open(IO::folder + IO::DBName + IO::patExt, std::ios::binary);
                // return outputKeys(IO::getKeys(file));
                return {"Not implemented yet"};
            case 'r':
                // price hash keys
                file.open(IO::folder + IO::DBName + IO::priceExt, std::ios::binary);
                break;
            case 'u':
                // publisher hash keys
                file.open(IO::folder + IO::DBName + IO::pubExt, std::ios::binary);
                break;
            default:
                break;
            }
            break;
        case 'r':
            // review hash keys
            file.open(IO::folder + IO::DBName + IO::reviewExt, std::ios::binary);
            break;
        case 't':
            // tag hash keys
            file.open(IO::folder + IO::DBName + IO::tagExt, std::ios::binary);
            break;
        default:
            break;
        }

        auto keys = IO::getKeys(file);
        file.close();
        return keys;
    }
}