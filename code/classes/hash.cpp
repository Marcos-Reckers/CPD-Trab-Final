#include "hash.hpp"

namespace Tables
{
    Hash::Hash() { this->hashTable = std::unordered_map<std::string, std::vector<int>>();}
    Hash::~Hash() { this->hashTable.clear();}
    size_t Hash::Size(){ return this->hashTable.size(); }
    void Hash::clear() { this->hashTable.clear(); }
    
    void Hash::Insert(const std::string &key, int data)
    {
        if (this->hashTable.find(key) == this->hashTable.end())
        {
            this->hashTable[key] = std::vector<int>();
        }
        this->hashTable[key].push_back(data);
    }

    std::vector<int> Hash::Search(const std::string &key)
    {
        auto it = this->hashTable.find(key);

        if (it != this->hashTable.end())
        {
            return it->second;
        }

        return std::vector<int>();
    }

    bool Hash::DeleteGame(const std::string &key, int Data)
    {
        for (size_t i = 0; i < this->hashTable[key].size(); i++)
        {
            if (this->hashTable[key][i] == Data)
            {
                this->hashTable[key].erase(this->hashTable[key].begin() + i, this->hashTable[key].begin() + i + 1);
                return true;
            }
        }
        return false;
    }

    bool Hash::Delete(const std::string &key)
    {
        auto it = this->hashTable.find(key);

        if (it != this->hashTable.end())
        {
            this->hashTable.erase(it);
            return true;
        }

        return false;
    }

    void Hash::Insert(const std::vector<std::string> &keys, int data)
    {
        for (auto item : keys)
        {
            if (item[0] == ' ' && item.size() > 1)
                item = item.substr(1, item.size() - 1);
            this->Insert(item, data);
        }
    }

    int Hash::writeToFile(std::ofstream &file)
    {
        for (auto item : this->hashTable)
        {
            file << item.first << ":";
            for (auto i : item.second)
            {
                file << i << " ";
            }
            file << std::endl;
        }
        return 0;
    }
}
