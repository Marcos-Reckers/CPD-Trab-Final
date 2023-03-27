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
    template <>
    int Hash<int>::writeToFile(std::ofstream &file)
    {
        int size = this->hashTable.size();
        file.write((char *)&size, sizeof(this->hashTable));
        for(auto item : this->hashTable)
        {
            file.write((char *)&item.first, sizeof(int));
            auto DataSize = item.second.size();
            file.write((char *)&DataSize, sizeof(int));
            for(auto data : item.second)
            {
                file.write((char *)&data, sizeof(int));
            }
        }
        return 0;
    }
    
    template <>
    int Hash<int>::readFromFile(std::ifstream &file)
    {
        int size = 0;
        file.read((char *)&size, sizeof(int));
        for(int i = 0; i < size; i++)
        {
            int key = 0;
            file.read((char *)&key, sizeof(int));
            std::vector<int> data;
            int dataSize = 0;
            file.read((char *)&dataSize, sizeof(int));
            for(int j = 0; j < dataSize; j++)
            {
                int dataItem = 0;
                file.read((char *)&dataItem, sizeof(int));
                data.push_back(dataItem);
            }
            this->hashTable[key] = data;
        }

        return 0;
    }
}
