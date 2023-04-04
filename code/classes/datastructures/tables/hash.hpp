#pragma once
#include "../../strings.hpp"

namespace Tables
{
    template <typename T>
    class Hash
    {
    private:
        size_t size = 0;

    public:
        std::unordered_map<T, std::vector<int>> hashTable;
        Hash() { this->hashTable = std::unordered_map<T, std::vector<int>>(); }
        Hash(std::ifstream &file)
        {
            this->hashTable = std::unordered_map<T, std::vector<int>>();
            this->readFromFile(file);
        }
        ~Hash() { this->hashTable.clear(); }
        void clear() { this->hashTable.clear(); }
        size_t Size() { return this->hashTable.size(); }
        void Insert(const T &key, int data)
        {
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
        
        bool DeleteGame(int Data)
        {
            for (auto it = this->hashTable.begin(); it != this->hashTable.end(); it++)
            {
                for (size_t i = 0; i < it->second.size(); i++)
                {
                    if (it->second[i] == Data)
                    {
                        it->second.erase(it->second.begin() + i, it->second.begin() + i + 1);
                        return true;
                    }
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

        int readFromFile(std::ifstream &file)
        {
            this->clear();
            std::string line;
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string key;
                std::getline(ss, key, ';');
                std::string value;
                while (std::getline(ss, value, ' '))
                {
                    if(value == "\r")
                        continue;
                        
                    this->Insert(key, std::stoi(value));
                }
                
            }
            return 0;
        }

        friend std::ostream &operator<<(std::ostream &os, const Hash &table)
        {
            for (auto n : table.hashTable)
            {
                os << n.first << ";";
                for (auto i : n.second)
                {
                    os << i << " ";
                }
                os << std::endl;
            }
            return os;
        }

        std::vector<std::string> GetKeys()
        {
            std::vector<std::string> keys;

            for (auto n : this->hashTable)
            {
                std::stringstream ss;

                ss << n.first;
                keys.push_back(ss.str());
            }

            return keys;
        }
    };

    template <>
    void Hash<std::string>::Insert(const std::vector<std::string> &keys, int data);
    template <>
    int Hash<int>::writeToFile(std::ofstream &file);
    template <>
    int Hash<int>::readFromFile(std::ifstream &file);
}
