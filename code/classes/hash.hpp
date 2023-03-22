#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

namespace Tables
{
    template <typename T>
    class Hash
    {
    private:
        size_t size = 0;

    public:
        std::unordered_map<T, std::vector<int>> hashTable;
        Hash() { this->hashTable = std::unordered_map<std::string, std::vector<int>>(); }
        ~Hash() { this->hashTable.clear(); }
        void clear() { this->hashTable.clear(); }
        size_t Size() { return this->hashTable.size(); }
        void Insert(const T &key, int data)
        {
            if (this->hashTable.find(key) == this->hashTable.end())
            {
                this->hashTable[key] = std::vector<int>();
            }
            this->hashTable[key].push_back(data);
        }
        std::vector<int> Search(const T &key)
        {
            auto it = this->hashTable.find(key);

            if (it != this->hashTable.end())
            {
                return it->second;
            }

            return std::vector<int>();
        }
        bool DeleteGame(const T &key, int Data)
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
        bool Delete(const T &key)
        {
            auto it = this->hashTable.find(key);

            if (it != this->hashTable.end())
            {
                this->hashTable.erase(it);
                return true;
            }

            return false;
        }

        void Insert(const std::vector<T> &keys, int data)
        {
            for (auto item : keys)
            {
                this->Insert(item, data);
            }
        }
        
        int writeToFile(std::ofstream &file)
        {
            file << *this;
            return 0;
        }
        friend std::ostream &operator<<(std::ostream &os, const Hash &table)
        {
            for (auto n : table.hashTable)
            {
                os << n.first << ":";
                for (auto i : n.second)
                {
                    os << i << " ";
                }
                os << std::endl;
            }
            return os;
        }
    };

    template <>
    void Hash<std::string>::Insert(const std::vector<std::string> &keys, int data);
}
