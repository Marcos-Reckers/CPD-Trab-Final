#include "classes/Trees/patricia.hpp"
#include "classes/game.hpp"
#include "classes/hash.hpp"
#include "classes/io.hpp"
#include "classes/Trees/b+.hpp"
#include <chrono>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Use -h to see commands" << std::endl;
        return 1;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
        args.emplace_back(argv[i]);

    if (args[0] == "-h")
    {
        std::cout << "Commands:" << std::endl;
        std::cout << "-convert <importPath> <limit> <exportPath> - Imports games from a csv file (-1 for all)" << std::endl;
        std::cout << "-get <path> <index> - Gets a game from a binary file" << std::endl;
        return 0;
    }
    else if (args[0] == "-c")
    {
        if (args.size() < 4)
        {
            std::cout << "Use -help to see commands" << std::endl;
            return 1;
        }

        auto begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto games = IO::importGames(args[1], std::stoi(args[2]));
        auto end = std::chrono::high_resolution_clock::now(); // Stop the timer.

        std::cout << "Games imported: " << games.size() << std::endl;
        std::cout << "Time to import: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        begin = std::chrono::high_resolution_clock::now(); // Start the timer.
        auto Bytes = IO::exportGames(args[3], games);
        end = std::chrono::high_resolution_clock::now(); // Stop the timer.

        std::cout << "Bytes exported: " << Bytes << " Bytes" << std::endl;
        std::cout << "Time to export: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

        return 0;
    }
    else if (args[0] == "-g")
    {
        if (args.size() < 3)
        {
            std::cout << "Use -help to see commands" << std::endl;
            return 1;
        }

        auto game = IO::getGame(args[1], std::stoi(args[2]));

        if (game.getAppid() == 0)
        {
            std::cout << "Index out of Range" << std::endl;
            return 1;
        }

        std::cout << game << std::endl;

        return 0;
    }
    else
    {
        std::cout << "Use -h to see commands" << std::endl;
        return 1;
    }
}