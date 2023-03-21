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

    std::vector<int> vectorFromListInt(const std::string &str)
    {
        std::vector<int> vector;                         // Vector to be returned.
        std::string Str = str.substr(1, str.size() - 2); // Removes '[' and ']'
        auto Strs = customSplit(Str, ',');               // Split the string by ','

        for (auto &String : Strs)
        {
            if (String[0] == ' ')
                String = String.substr(1, String.size()); // Remove the first space
            vector.push_back(std::stoi(String));
        }
        return vector;
    }

    std::vector<std::string> vectorFromListStr(const std::string &str)
    {
        std::vector<std::string> vector;                 // Vector to be returned.
        std::string Str = str.substr(1, str.size() - 2); // Removes '[' and ']'
        auto Strs = customSplit(Str, ',');               // Split the string by ','

        for (auto &String : Strs)
        {
            if (String[0] == ' ')
                String = String.substr(1, String.size()); // Remove the first space
            vector.push_back(String);
        }
        return vector;
    }

    std::vector<bool> vectorFromListBool(const std::string &str)
    {
        std::vector<bool> vector;                        // Vector to be returned.
        std::string Str = str.substr(1, str.size() - 2); // Removes '[' and ']'
        auto Strs = customSplit(Str, ',');               // Split the string by ','

        for (auto &String : Strs)
        {
            if (String[0] == ' ')
                String = String.substr(1, String.size()); // Remove the first space
            vector.push_back(String == "True");
        }
        return vector;
    }
}