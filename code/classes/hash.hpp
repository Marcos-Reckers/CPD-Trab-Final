#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

namespace Tables
{
    class Hash
    {
    private:
        size_t size = 0;

    public:
        std::unordered_map<std::string, std::vector<int>> hashTable;
        Hash();
        ~Hash();
        void clear();
        size_t Size();
        void Insert(const std::string &key, int data);
        std::vector<int> Search(const std::string &key);
        bool DeleteGame(const std::string &key, int Data);
        bool Delete(const std::string &key);
        void Insert(const std::vector<std::string> &keys, int data);
        int writeToFile(std::ofstream &file);
        friend std::ostream &operator<<(std::ostream &os, const Hash &table)
        {
            for(auto n : table.hashTable)
            {
                os << "Key: " << n.first << " Data: ";
                for(auto i : n.second)
                {
                    os << i << " ";
                }
                os << std::endl;
            }
            return os;
        }
    };
    
} 
