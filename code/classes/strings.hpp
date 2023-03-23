#pragma once
#include "includes.hpp"

namespace STR
{
    std::vector<std::string> customSplit(const std::string &str, char separator);

    template <typename T>
    std::vector<T> vectorFromList(const std::string &str);
}