#include "io.hpp"

namespace IO
{
    std::vector<std::string> customSplit(const std::string& str, char separator)
    {
        std::vector<std::string> strings;
        int startIndex = 0, endIndex = 0;
        for (size_t i = 0; i <= str.size(); i++)
        {

            // If we reached the end of the word or the end of the input.
            if (str[i] == separator || i == str.size())
            {
                endIndex = i;
                std::string temp;
                temp.append(str, startIndex, endIndex - startIndex);
                strings.push_back(temp);
                startIndex = endIndex + 1;
            }
        }
        return strings;
    }

    std::vector<DB::Game> importGames(const std::string &path, size_t limit)
    {
        std::vector<DB::Game> games; // Vector of games to be returned.
        std::ifstream myfile;        // File to be read.
        myfile.open(path);           // Open the file.

        if (!myfile.is_open() || !myfile.good())
        {
            std::cout << "Error opening file" << std::endl;
            return games;
        }

        const int urlIndex = 0;
        const int nameIndex = 1;
        const int reviewIndex = 2;
        const int releaseDateIndex = 3;
        const int developerIndex = 4;
        const int publisherIndex = 5;
        const int popularTagsIndex = 6;
        const int gameDetailsIndex = 7;
        const int languagesIndex = 8;
        const int genreIndex = 9;
        const int priceIndex = 10;

        std::string line;           // Line to be read.
        std::getline(myfile, line); // Skip the first line.
        std::getline(myfile, line); // Skip the second line.

        while (!myfile.eof() && games.size() < limit) // While we haven't reached the end of the file.
        {
            std::getline(myfile, line);            // Read the next line.
            auto strings = customSplit(line, ';'); // Split the line into a vector of strings.
            // Add the tags to the game.
            const std::string tags = strings[popularTagsIndex] + ", " + strings[gameDetailsIndex] + ", " + strings[languagesIndex] + ", " + strings[genreIndex];
            // Temporary game to be added to the vector.
            const auto temp = DB::Game(strings[urlIndex], strings[nameIndex], strings[developerIndex],
                                       strings[publisherIndex], strings[releaseDateIndex], tags,
                                       strings[priceIndex], strings[reviewIndex]);
            games.push_back(temp); // Add the game to the vector.
        }

        myfile.close();

        return games;
    }

    int exportGames(const std::string &path, const std::vector<DB::Game> &games)
    {
        std::ofstream file(path, std::ios::binary); // File to be written to.
        size_t Filesize = 0; // Size of the file.

        if(!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return -1;
        }

        if(!file.good())
        {
            std::cout << "Error writing to file" << std::endl;
            return -1;
        }

        for (auto game : games)
        {
            Filesize += game.writeToFile(file); // Write the game to the file.
        }

        file.close();

        return Filesize;
    }

    DB::Game getGame(const std::string &path, size_t index)
    {
        std::ifstream file(path, std::ios::binary);

        if(!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return DB::Game();
        }

        if(!file.good())
        {
            std::cout << "Error reading from file" << std::endl;
            return DB::Game();
        }

        DB::Game game;

        return game.readFromFile(file, index) ? game : DB::Game();
    }
}