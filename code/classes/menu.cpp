#include "menu.hpp"

namespace MENU
{
    int HelpMessage()
    {
        std::cout << "Use -h to see commands" << std::endl;
        return 1;
    }

    int Help(const std::vector<std::string> &args)
    {
        std::cout << "Commands:" << std::endl;
        std::cout << "-c <importPath> <limit> - Imports games from a csv file to binary files (-1 for all)" << std::endl;
        std::cout << "-g <Index> - Searches a game from a binary file" << std::endl;
        std::cout << "-s [<Index>, <Index>] - Searches for a game in a binary file" << std::endl;
        std::cout << "-a <game> - Appends a game to a binary file" << std::endl;
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

        auto gamesID = STR::vectorFromListInt(args[2]);

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

        auto genres = STR::vectorFromListStr(strings[0].substr(4));
        auto languages = STR::vectorFromListStr(strings[1].substr(4));
        auto tags = STR::vectorFromListStr(strings[2].substr(4));
        auto dates = STR::vectorFromListInt(strings[3].substr(4));
        auto developers = STR::vectorFromListStr(strings[4].substr(4));
        auto publishers = STR::vectorFromListStr(strings[5].substr(4));
        std::cout << strings[8].substr(4).length() << std::endl;
        auto minPrice = std::stof(strings[6].substr(4));
        auto maxPrice = std::stof(strings[7].substr(4));
        auto decade = strings[8].substr(4).length() ? std::stoi(strings[8].substr(4)) : -1;
        auto reviews = STR::vectorFromListBool(strings[9].substr(4));
        auto gameID = strings[10].substr(4).length() ? std::stoi(strings[10].substr(4)) : -1;
        auto name = strings[11].substr(4);

        //! Debug, seeing for the correct substrings
        print(genres, "Genres");
        print(languages, "Languages");
        print(tags, "Tags");
        print(dates, "Dates");
        print(developers, "Developers");
        print(publishers, "Publishers");
        std::cout << "Min Price: " << minPrice << std::endl;
        std::cout << "Max Price: " << maxPrice << std::endl;
        std::cout << "Decade: " << decade << std::endl;
        print(reviews, "Reviews");
        std::cout << "Game ID: " << gameID << std::endl;
        std::cout << "Name: " << name << std::endl;

        return 0;
    }
}