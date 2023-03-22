#pragma once
#include <chrono>
#include <tuple>
#include "io.hpp"

namespace MENU
{
    int HelpMessage();

    int Help(const std::vector<std::string> &args);

    int ImpExp(const std::vector<std::string> &args);

    int Get(const std::vector<std::string> &args);

    int Set(const std::vector<std::string> &args);

    int Append(const std::vector<std::string> &args);

    int Load(const std::vector<std::string> &args);

    int Convert(const std::vector<std::string> &args);

    int Search(const std::vector<std::string> &args);
}