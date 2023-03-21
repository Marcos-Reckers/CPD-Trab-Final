#pragma once
#include <chrono>
#include "io.hpp"

namespace MENU
{
    const std::string DBPath = "Steam.db"; // Path to the database.

    int HelpMessage();

    int Help(const std::vector<std::string> &args);

    int Convert(const std::vector<std::string> &args);

    int Get(const std::vector<std::string> &args);

    int Set(const std::vector<std::string> &args);

    int Append(const std::vector<std::string> &args);

    int Load(const std::vector<std::string> &args);
}