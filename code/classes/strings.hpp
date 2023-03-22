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

    template <typename T>
    std::vector<T> vectorFromList(const std::string &str);
}