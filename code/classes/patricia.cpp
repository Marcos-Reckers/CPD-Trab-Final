#include "patricia.hpp"

namespace Trees
{

    void PatriciaTree::Node::print()
    {
        std::cout << "Key: " << key << std::endl;
        std::cout << "Value: ";
        for (size_t i = 0; i < value.size(); i++)
        {
            std::cout << value[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Children: ";
        for (size_t i = 0; i < children.size(); i++)
        {
            children[i]->print();
        }

        if (children.empty())
        {
            std::cout << "NULL";
        }
        std::cout << std::endl;
    }

    int PatriciaTree::Node::AddNode(const Node &node)
    {
        if(node.key == this->key)
        {
            this->value.push_back(node.value); 
        }
        return 1;
    }

    PatriciaTree::Node::Node(const std::string &Key) : key(Key)
    {
        value = std::vector<int>();
        children = std::vector<Node *>();
    }

    PatriciaTree::Node::~Node()
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
        children.clear();
        value.clear();
    }

    PatriciaTree::PatriciaTree()
    {
        root = std::vector<PatriciaTree::Node *>();
    }

    PatriciaTree::~PatriciaTree()
    {
        for (auto &i : root)
        {
            delete i;
        }
        root.clear();
    }

    void PatriciaTree::insert(std::string key, int value)
    {
        // Convert the key to lowercase.
        for (auto &c : key)
            c = tolower(c);

        // If the tree is empty, create a new node and add it to the root.
        if (root.empty())
        {
            Node *temp = new Node(key);
            temp->value.push_back(value);
            root.push_back(temp);
            return;
        }

        // If the tree is not empty, search for the key.
        for (size_t i = 0; i < root.size(); i++)
        {
            // If the key is found, add the value to the node.
            if (root[i]->key == key)
            {
                root[i]->value.push_back(value);
                return;
            }
            // If the key is not found, find the first different character.
            else
            {
                size_t j = 0;
                while (j < root[i]->key.size() && j < key.size() && root[i]->key[j] == key[j])
                {
                    j++;
                }
                if (j == key.size())
                {
                    Node *temp = new Node(key);
                    temp->value.push_back(value);
                    ch.push_back(temp);
                    return;
                }
            }
        }
    }

    void PatriciaTree::remove(std::string key)
    {
        // Convert the key to lowercase.
        for (auto &c : key)
            c = tolower(c);

        // If the tree is empty, return.
        if (root.empty())
            return;

        // If the tree is not empty, search for the key.
        for (size_t i = 0; i < root.size(); i++)
        {
            // If the key is found, remove the node.
            if (root[i]->key == key)
            {
                root.erase(root.begin() + i);
                return;
            }
        }
    }

    int PatriciaTree::search(std::string key)
    {
        // Convert the key to lowercase.
        for (auto &c : key)
            c = tolower(c);

        // If the tree is empty, return -1.
        if (root.empty())
            return -1;

        // If the tree is not empty, search for the key.
        for (size_t i = 0; i < root.size(); i++)
        {
            // If the key is found, return the value.
            if (root[i]->key == key)
            {
                return root[i]->value[0];
            }
        }

        // If the key is not found, return -1.
        return -1;
    }
}