#include "strings.hpp"

namespace STR
{
    std::vector<std::string> customSplit(const std::string &str, char separator)
    {
        std::vector<std::string> strings; // Vector of strings to be returned.
        int startIndex = 0, endIndex = 0; // Start and end index of the word.
        for (size_t i = 0; i <= str.size(); i++)
        {
            // If we reached the end of the word or the end of the input.
            if (str[i] == separator || i == str.size())
            {
                endIndex = i;                                        // Set the end index to the current index.
                std::string temp;                                    // Temp string to be added to the vector.
                temp.append(str, startIndex, endIndex - startIndex); // Add the word to the string.
                strings.push_back(temp);                             // Add the string to the vector.
                startIndex = endIndex + 1;                           // Set the start index to the next word.
            }
        }
        return strings;
    }

    int removeAnomalies(std::string &str)
    {
        if (str[0] == ' ')
            str = str.substr(1, str.size()); // Remove the first space

        if (str[0] == '\'' || str[0] == '\"')
            str = str.substr(1, str.size()); // Remove the first and last character

        if (str[str.size() - 1] == '\'' || str[str.size() - 1] == '\"')
            str = str.substr(0, str.size() - 1); // Remove the first and last character

        return 0;
    }
}