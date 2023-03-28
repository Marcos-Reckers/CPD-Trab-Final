#pragma once
#include "classes/io.hpp"

namespace database
{
    // int Append(const std::vector<std::string> &args);

    std::string convert(const std::string &path, int size);

    std::vector<std::vector<std::string>> search(const std::vector<std::string> &genres,
                                                 const std::vector<std::string> &languages,
                                                 const std::vector<std::string> &tags,
                                                 const std::vector<std::string> &dates,
                                                 const std::vector<std::string> &developers,
                                                 const std::vector<std::string> &publishers,
                                                 const std::vector<std::string> &reviews,
                                                 const std::string &name,
                                                 int minPrice, int maxPrice, int decade, int appID, bool searchType);

    bool validate(void);

    std::vector<std::string> getField(const std::string &field);
}