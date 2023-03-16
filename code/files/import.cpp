#include "import.hpp"

namespace cpd
{
    std::vector<std::string> customSplit(std::string str, char separator)
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

    std::vector<Game> importGames(std::string path, size_t limit)
    {
        std::vector<Game> games;
        std::ifstream myfile;
        myfile.open(path);
        if (!myfile.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return games;
        }
        std::string line;
        std::getline(myfile, line);
        std::getline(myfile, line);
        while (!myfile.eof() && games.size() < limit)
        {
            std::getline(myfile, line);
            auto strings = cpd::customSplit(line, ';');
            cpd::Game temp = cpd::Game();
            temp = cpd::Game(strings[0], strings[1], strings[4], strings[5], strings[3], strings[6], strings[2], strings[10]);
            games.push_back(temp);
        }
        myfile.close();
        return games;
    }
}