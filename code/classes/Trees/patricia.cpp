#include "patricia.hpp"

namespace Trees
{
    size_t Compare(const std::string &str1, const std::string &str2)
    {
        size_t i = 0;
        while (i < str1.length() && i < str2.length() && str1[i] == str2[i])
        {
            i++;
        }
        return i;
    }

    Patricia::Node::Node() : data(-1), key(), children() {}
    Patricia::Node::Node(const std::string &Key, int Data) : data(Data), key(Key), children() {}
    Patricia::Node::Node(const Node &node) : data(node.data), key(node.key), children(node.children) {}
    Patricia::Node::~Node() { this->children.clear(); }
    int Patricia::Node::GetData() { return this->data; }
    std::string Patricia::Node::GetKey() { return this->key; }

    bool Patricia::Node::SetData(int Data)
    {
        this->data = Data;
        return true;
    }

    Patricia::Node *Patricia::Node::GetChild(size_t index)
    {
        if (index >= this->children.size())
            return nullptr; // Index out of bounds

        return this->children[index];
    }

    Patricia::Node *Patricia::Node::Insert(const std::string &key, int Data)
    {
        for (auto child : this->children)
        {
            if (child->key == key)
            {
                child->data = Data;
                return child;
            }
            auto i = Compare(key, child->key);
            if (i == 0)
            {
                // nenhum e substring do outro
                continue;
            }
            else if (i > key.length())
            {
                // key é substring de child
                // bol -> bola
                auto newNode = new Node(*child);
                newNode->key = child->key.substr(i, child->key.length() - i);
                child->key = key;
                child->data = Data;
                child->children.clear();
                child->children.push_back(newNode);
                return child;
            }
            else if (i == child->key.length())
            {
                // child e substring de key
                // bolacha -> bola
                return child->Insert(key.substr(i, key.length() - i), Data);
            }
            else if (i > 0 && i < key.length() && i < child->key.length())
            {
                // key e child tem uma substring em comum
                // bolo -> bola
                auto newNode = new Node(key.substr(i, key.length() - i), Data);
                auto newNode1 = new Node(*child);
                newNode1->key = child->key.substr(i, child->key.length() - i);
                child->key = key.substr(0, i);
                child->data = -1;
                child->children.clear();
                child->children.push_back(newNode);
                child->children.push_back(newNode1);
                return newNode;
            }
        }
        // nao e substring de nenhum filho
        auto newNode = new Node(key, Data);
        this->children.push_back(newNode); // Add the new node as a child of the root
        return newNode;                    // Return the new node
    }

    Patricia::Node *Patricia::Node::Search(const std::string &Key)
    {
        if (this->key == Key)
        {
            return this; // Return the current node
        }
        else if (Key.starts_with(this->key))
        {
            for (auto child : this->children)
            {
                auto temp = child->Search(Key.substr(this->key.length(), Key.length() - this->key.length())); // Search for the node to search for in the child
                if (temp != nullptr)                                                                          // If the node was found
                {
                    return temp; // Return the node
                }
            }
            return nullptr; // Return null because the node was not found
        }

        return nullptr; // Return null because the node was not found
    }

    void Patricia::Node::clear()
    {
        for (auto child : this->children)
            child->clear(); // Remove the child

        delete this; // Remove the root
    }

    std::vector<int> Patricia::Node::GetChildrenData()
    {
        std::vector<int> childrenData;
        for (auto child : this->children)
        {
            for (auto childData : child->GetChildrenData())
            {

                childrenData.push_back(childData);
            }
        }
        if (this->data != -1)
            childrenData.push_back(this->data);
        return childrenData;
    }

    void Patricia::clear()
    {
        for (auto child : this->root)
        {
            child->clear(); // Remove the child
        }

        this->root.clear(); // Clear the root
    }

    Patricia::Patricia()
    {
        this->root = std::vector<Node *>(); // Initialize the root
        this->size = 0;                     // Initialize the size
    }

    Patricia::~Patricia()
    {
        this->clear(); // Clear the tree
    }

    size_t Patricia::Size()
    {
        return this->size; // Return the size
    }

    Patricia::Node *Patricia::Insert(const std::string &Key, int Data)
    {
        this->size++;
        std::string key = Key;
        for (auto &c : key)
            c = std::tolower(c);

        for (auto child : this->root)
        {
            if (child->key == key)
            {
                child->data = Data;
                return child;
            }
            auto i = Compare(key, child->key);
            if (i == 0)
            {
                // nenhum e substring do outro
                continue;
            }
            else if (i > key.length())
            {
                // key é substring de child
                // bol -> bola
                auto newNode = new Node(*child);
                newNode->key = child->key.substr(i, child->key.length() - i);
                child->key = key;
                child->data = Data;
                child->children.clear();
                child->children.push_back(newNode);
                return child;
            }
            else if (i == child->key.length())
            {
                // child e substring de key
                // bolacha -> bola
                return child->Insert(key.substr(i, key.length() - i), Data);
            }
            else if (i > 0 && i < key.length() && i < child->key.length())
            {
                // key e child tem uma substring em comum
                // bolo -> bola
                auto newNode = new Node(key.substr(i, key.length() - i), Data);
                auto newNode1 = new Node(*child);
                newNode1->key = child->key.substr(i, child->key.length() - i);
                child->key = key.substr(0, i);
                child->data = -1;
                child->children.clear();
                child->children.push_back(newNode);
                child->children.push_back(newNode1);
                return newNode;
            }
        }
        // nao e substring de nenhum filho
        auto newNode = new Node(key, Data);
        this->root.push_back(newNode); // Add the new node as a child of the root
        return newNode;                // Return the new node
    }

    Patricia::Node *Patricia::SearchNode(const std::string &Key)
    {
        std::string key = Key;
        for (auto &c : key)
            c = std::tolower(c);

        for (auto child : this->root)
        {
            if (key.starts_with(child->key))
            {
                auto node = child->Search(key); // Search for the node
                if (node != nullptr)            // If the node was found
                {
                    return node; // Return the data of the node
                }
            }
        }
        return nullptr; // Return nullptr because the node was not found
    }

    int Patricia::Search(const std::string &Key)
    {
        auto node = this->SearchNode(Key); // Search for the node
        if (node != nullptr)               // If the node was found
        {
            return node->data; // Return the data of the node
        }
        return -1; // Return -1 because the node was not found
    }

    std::vector<int> Patricia::SearchPrefix(const std::string &prefix)
    {
        auto firstOcurrence = this->SearchNode(prefix); // Search for the first ocurrence of the prefix

        if (firstOcurrence == nullptr) // If the first ocurrence was not found
            return std::vector<int>(); // Return an empty vector

        return firstOcurrence->GetChildrenData(); // Return the data of the children of the first ocurrence
    }

    bool Patricia::Delete(const std::string &Key)
    {
        auto toDelete = this->SearchNode(Key); // Search for the node to delete
        toDelete->data = -1;                   // Set the data of the node to delete to -1
        return true;                           // Return true because the operation succeeded
    }

    // int Patricia::Node::writeSizeToFile(std::ofstream &file)
    // {
    //     int size = 0;
    //     file.write((char *)this->children.size(), sizeof(this->children.size()));
    //     size += sizeof(this->children.size());

    //     for(auto child : this->children)
    //     {
    //         size += child->writeSizeToFile(file);
    //     }

    //     return size;
    // }

    // int Patricia::Node::writeToFile(std::ofstream &file)
    // {

    //     int size = 0;
    
    //     file.write((char *)&this->key, sizeof(this->key));
    //     file.write((char *)&this->data, sizeof(this->data));
    //     size += sizeof(this->key) + sizeof(this->data);

    //     for (auto child : this->children)
    //     {
    //         size += child->writeToFile(file);
    //     }
    //     return size;
    // }

    // int Patricia::writeToFile(std::ofstream &file)
    // {
    //     int size = 0;
    //     file.write((char*)this->size, sizeof(this->size));
    //     size += sizeof(this->size);
    //     file.write((char*)this->root.size(), sizeof(this->root.size()));
    //     size += sizeof(this->root.size());

    //     for (auto child : this->root)
    //     {
    //         size += child->writeSizeToFile(file);
    //     }

    //     for (auto child : this->root)
    //     {
    //         size += child->writeToFile(file);
    //     }

    //     return size;
    // }

    // int Patricia::Node::readFromFile(std::ifstream &file)
    // {
    //     int size = 0;
    //     std::string key;
    //     int data;
    //     while (file.read((char *)&key, sizeof(key)))
    //     {
    //         file.read((char *)&data, sizeof(data));
    //         this->Insert(key, data);
    //         size += sizeof(key) + sizeof(data);
    //     }
    //     return size;
    // }

    // int Patricia::readFromFile(std::ifstream &file)
    // {
    //     int size = 0;
    //     std::string key;
    //     int data;
    //     while (file.read((char *)&key, sizeof(key)))
    //     {
    //         file.read((char *)&data, sizeof(data));
    //         this->Insert(key, data);
    //         size += sizeof(key) + sizeof(data);
    //     }
    //     return size;
    // }
}