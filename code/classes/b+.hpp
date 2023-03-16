#pragma once
#include <iostream>
#include <string>

namespace Trees
{
    class BPlusTree
    {
        public:
            BPlusTree();
            ~BPlusTree();
            void insert(const std::string& key, int appid);
            void remove(const std::string &key);
            int search(const std::string &key);
            void print();
        private:
            struct Node
            {
                const std::string &key;
                int appid;
                Node* left;
                Node* right;
            };
            Node* root;
    };
}