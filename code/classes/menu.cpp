#include "menu.hpp"

namespace MENU
{
    int HelpMessage()
    {
        std::cout << "Use -h to see commands" << std::endl;
        return -1;
    }

    int Help(const std::vector<std::string> &args)
    {
        std::cout << "Commands:" << std::endl;
        std::cout << "-c <importPath> <limit> - Imports games from a csv file to binary files (-1 for all)" << std::endl;
        std::cout << "-g <Index> - Searches a game from a binary file" << std::endl;
        std::cout << "-s <Attributes> - Searches for games in a binary file" << std::endl;
        std::cout << "-a <game> - Appends a game to a binary file" << std::endl;
        std::cout << "-v - Validates if all the data is in place" << std::endl;
        std::cout << "-f <Name> - Gets all keys from a particular structure" << std::endl;
        return 0;
    }

    int ImpExp(const std::vector<std::string> &args)
    {
        if (args.size() < 3)
        {
            return HelpMessage();
        }

        auto begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto imp = IO::importGames(args[1], std::stoi(args[2]));
        auto end = std::chrono::high_resolution_clock::now(); // Stop the timer.

        std::cout << "Games imported: " << imp.size() << std::endl;
        std::cout << "Time to import: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto Bytes = IO::exportGames(IO::folder + IO::DBName, imp);
        end = std::chrono::high_resolution_clock::now(); // Stop the timer.

        std::cout << "Bytes exported: " << Bytes << " Bytes" << std::endl;
        std::cout << "Time to export: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        return 0;
    }

    int Get(const std::vector<std::string> &args)
    {
        if (args.size() < 3)
        {
            return HelpMessage();
        }

        auto game = IO::getGame(IO::folder + IO::DBName, std::stoi(args[2]));

        if (game.getAppid() == 0)
        {
            std::cout << "Index out of Range" << std::endl;
            return 1;
        }

        std::cout << game << std::endl;

        return 0;
    }

    int Set(const std::vector<std::string> &args)
    {
        if (args.size() < 2)
        {
            return HelpMessage();
        }

        auto gamesID = STR::vectorFromList<int>(args[2]);

        for (auto &gameID : gamesID)
        {
            auto game = IO::getGame(IO::folder + IO::DBName, gameID);

            if (game.getAppid() == 0)
            {
                std::cout << "No Game found at Index" << gameID << std::endl;
            }

            std::cout << game << std::endl;
        }

        return 0;
    }

    int Append(const std::vector<std::string> &args)
    {
        if (args.size() < 3)
        {
            return HelpMessage();
        }

        auto game = DB::Game();
        auto index = IO::appendGame(IO::folder + IO::DBName, game);
        if (index == 0)
        {
            std::cout << "Couldn't append game" << std::endl;

            return 1;
        }

        std::cout << "Game Index: " << index << std::endl;
        return 0;
    }

    int Load(const std::vector<std::string> &args)
    {
        if (args.size() < 1)
        {
            return HelpMessage();
        }

        auto begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto games = IO::loadGames(IO::folder + IO::DBName);
        auto end = std::chrono::high_resolution_clock::now(); // Stop the timer.

        std::cout << "Games loaded: " << games.size() << std::endl;
        std::cout << "Time to load: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        return 0;
    }

    int Convert(const std::vector<std::string> &args)
    {
        if (args.size() < 3)
        {
            return HelpMessage();
        }
        auto begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto temp = IO::ConvertDatabase(args[1], std::stoi(args[2]));
        auto end = std::chrono::high_resolution_clock::now(); // Stop the timer.

        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        return temp;
    }

    template <typename T>
    void print(const std::vector<T> &vec, const std::string &name)
    {
        std::cout << name << ": ";
        bool first = true;
        for (auto item : vec)
        {
            first ? std::cout << item : std::cout << ", " << item;
            first = false;
        }
        std::cout << std::endl;
    }

    std::vector<int> searchFile(const std::string &path, const std::vector<std::string> &keys)
    {
        std::vector<int> ids;
        std::ifstream file;
        file.open(path, std::ios::binary);

        if(!file.good())
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

        if(minPrice > maxPrice)
            std::swap(minPrice, maxPrice);

        if(maxPrice == -1)
            maxPrice = INT_MAX;

        auto id = IO::searchFilePrice(file, minPrice, maxPrice);

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

        auto id = IO::searchFileDec(file, Decade);

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

    std::vector<int> intersection(const std::vector<int> &a, const std::vector<int> &b, int type)
    {
        std::unordered_set<int> temp;
        for(auto i : a)
        {
            for(auto j : b)
            {
                if (i == j)
                {
                    temp.insert(i);
                }
                else if(type)
                {
                    temp.insert(i);
                    temp.insert(j);
                }
            }
        }

        return std::vector<int>(temp.begin(), temp.end());
    }

    int Search(const std::vector<std::string> &args)
    {
        if (args.size() < 2)
        {
            return HelpMessage();
        }

        // GUI TEST CASES
        // gen=['Accounting'];lan=['Arabic'];tag=['1980s'];dat=[1960];dev=['(Miwashiba)'];pub=['!Lim studio'];min=43.33;max=416.67;dec=1970;rev=[True, True, True, True, True, True, True, True, True];gid=123214;nam=BAHA
        // gen=[];lan=[];tag=[];dat=[];dev=[];pub=[];min=0.0;max=500.0;dec=;rev=[False, False, False, False, False, False, False, False, False];gid=;nam=
        // gen=['Accounting', 'Action'];lan=['Arabic', 'Bulgarian'];tag=['1980s', "1990's"];dat=[1960, 1961];dev=['(Miwashiba)', ' Alexander Cobleigh'];pub=['!Lim studio', '#workshop'];min=26.67;max=480.0;dec=1970;rev=[True, True, True, True, True, True, True, True, True];gid=123123;nam=baha

        auto &search = args[1];
        auto strings = STR::customSplit(search, ';');

        // offset 4 to remove the "gen=" part
        auto type = std::stoi(strings[12].substr(4));

        auto gameID = strings[10].substr(4).length() ? std::stoi(strings[10].substr(4)) : -1;
        if (gameID != -1 && type == 0)
        {
            auto games = searchGames({gameID});
            if (games.size() == 0)
            {
                std::cout << "No games with that ID found." << std::endl;
                return 0;
            }
            std::cout << games[0] << std::endl;
            return 0;
        }

        auto genres = STR::vectorFromList<std::string>(strings[0].substr(4));
        auto languages = STR::vectorFromList<std::string>(strings[1].substr(4));
        auto tags = STR::vectorFromList<std::string>(strings[2].substr(4));
        auto dates = STR::vectorFromList<std::string>(strings[3].substr(4));
        auto developers = STR::vectorFromList<std::string>(strings[4].substr(4));
        auto publishers = STR::vectorFromList<std::string>(strings[5].substr(4));
        auto minPrice = std::stoi(strings[6].substr(4));
        auto maxPrice = std::stoi(strings[7].substr(4));
        auto decade = strings[8].substr(4).length() ? std::stoi(strings[8].substr(4)) : -1;
        auto reviews = STR::vectorFromList<std::string>(strings[9].substr(4));
        auto name = strings[11].substr(4);

        // //! Debug, seeing for the correct substrings
        // print(genres, "Genres");
        // print(languages, "Languages");
        // print(tags, "Tags");
        // print(dates, "Dates");
        // print(developers, "Developers");
        // print(publishers, "Publishers");
        // std::cout << "Min Price: " << minPrice << std::endl;
        // std::cout << "Max Price: " << maxPrice << std::endl;
        // std::cout << "Decade: " << decade << std::endl;
        // print(reviews, "Reviews");
        // std::cout << "Game ID: " << gameID << std::endl;
        // std::cout << "Name: " << name << std::endl;

        std::array<std::pair<std::vector<int>, bool>, 12> appids;
        for(auto &i : appids)
            i.second = false;

        if(genres.size() != 0)
            appids[0] = {searchFile(IO::folder + IO::DBName + IO::genreExt, genres), true}; // genres Ids

        if (languages.size() != 0)
            appids[1] = {searchFile(IO::folder + IO::DBName + IO::langExt, languages), true}; // languages Ids

        if (tags.size() != 0)
            appids[2] = {searchFile(IO::folder + IO::DBName + IO::tagExt, tags), true}; // tags Ids

        if(dates.size() != 0)
            appids[3] = {searchFile(IO::folder + IO::DBName + IO::dateExt, dates), true}; // dates Ids

        if (developers.size() != 0)
            appids[4] = {searchFile(IO::folder + IO::DBName + IO::devExt, developers), true}; // developers Ids

        if (publishers.size() != 0)
            appids[5] = {searchFile(IO::folder + IO::DBName + IO::pubExt, publishers), true}; // publishers Ids
        
        if(minPrice != -1 || maxPrice != -1) //Todo change the max price to a variable
            appids[6] = {searchFilePrice(IO::folder + IO::DBName + IO::priceExt, minPrice, maxPrice), true}; // price Ids

        if(decade != -1)
            appids[7] = {searchFileDec(IO::folder + IO::DBName + IO::dateExt, decade), true}; // decade Ids

        if(reviews.size() != 0)
            appids[8] = {searchFile(IO::folder + IO::DBName + IO::reviewExt, reviews), true}; // reviews Ids

        // if(name.length() != 0)
        //     appids[10] = {searchFile(IO::folder + IO::DBName + IO::nameExt, name), true}; // name Ids

        std::vector<int> Ids;
        bool first = true;
        for(auto &i : appids)
        {
            if (i.second && first)
            {
                first = false;
                Ids = i.first;
            }

            if (i.second)
            {
                Ids = intersection(Ids, i.first, type);
            }
        }

        auto games = searchGames(Ids);
        if (games.size() == 0)
        {
            std::cout << "No games found." << std::endl;
            return 0;
        }
        for(auto &i : games)
            std::cout << i << std::endl;
        return 0;
    }

    int Validate(const std::vector<std::string> &args)
    {
        if (args.size() < 1)
        {
            return HelpMessage();
        }

        std::cout << IO::databaseExists();

        return IO::databaseExists();
    }

    int outputKeys(const std::vector<std::string> &keys)
    {
        std::stringstream ss;
        for (auto key : keys)
        {
            ss << key << ";";
        }
        std::cout << ss.str() << std::endl;
        return 0;
    }

    int GetField(const std::vector<std::string> &args)
    {
        if (args.size() < 2)
        {
            return HelpMessage();
        }

        auto &field = args[1];

        std::ifstream file;

        switch (field[0])
        {
        case 'd':
            switch(field[1])
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
                std::cout << "Not implemented yet" << std::endl;
                return 1;
            case 'r':
                // price hash keys
                file.open(IO::folder + IO::DBName + IO::priceExt, std::ios::binary);
                break;
            case 'u':
                // publisher hash keys
                file.open(IO::folder + IO::DBName + IO::pubExt, std::ios::binary);
                break;
            default:
                return HelpMessage();
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
            return HelpMessage();
        }
        auto keys = IO::getKeys(file);
        file.close();
        return outputKeys(keys);
    }
}