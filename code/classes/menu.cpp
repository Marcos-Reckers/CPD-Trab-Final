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
}