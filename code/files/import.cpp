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
    std::vector<Game> importGames(std::string path)
    {
        
    }
}