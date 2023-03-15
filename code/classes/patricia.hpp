#pragma once
#include <iostream>
#include <string>
#include <vector>
namespace Trees
{
    class PatriciaTree
    {
    private:
        struct Node
        {
            std::string key;
            std::vector<int> value;
            std::vector<Node *> children;
            void print();
            int AddNode(const Node &node);
            Node(const std::string &Key);
            ~Node();
        };
        std::vector<Node *> root;

    public:
        PatriciaTree();
        ~PatriciaTree();
        void insert(std::string key, int value);
        void remove(std::string key);
        int search(std::string key);
        friend std::ostream &operator<<(std::ostream &os, const PatriciaTree &tree)
        {
            os << "Patricia Tree:" << std::endl;
            for (size_t i = 0; i < tree.root.size(); i++)
            {
                tree.root[i]->print();
            }
            return os;
        };
    };
}