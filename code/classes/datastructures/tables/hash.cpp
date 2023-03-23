#include "hash.hpp"

namespace Tables
{
    template <>
    void Hash<std::string>::Insert(const std::vector<std::string> &keys, int data)
    {
        for (auto item : keys)
        {
            if (item[0] == ' ' && item.size() > 1)
                item = item.substr(1, item.size() - 1);
            this->Insert(item, data);
        }
    }
}
