#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <algorithm>

namespace STR
{
    std::vector<std::string> customSplit(const std::string &str, char separator);

    std::vector<int> vectorFromListInt(const std::string &str);
    std::vector<std::string> vectorFromListStr(const std::string &str);
    std::vector<bool> vectorFromListBool(const std::string &str);
}