#include "patricia.hpp"

namespace Trees
{
    Patricia::Node::Node() : data(-1), key(), children() {}
    Patricia::Node::Node(const std::string &Key, int Data) : data(Data), key(Key), children() {}
    Patricia::Node::Node(const Node &node) : data(node.data), key(node.key), children(node.children) {}
    Patricia::Node::~Node() { this->children.clear(); }
    int Patricia::Node::GetData() { return this->data; }
    std::string Patricia::Node::GetKey() { return this->key; }

    bool Patricia::Node::SetData(int data)
    {
        this->data = data;
        return true;
    }

    Patricia::Node *Patricia::Node::GetChild(size_t index)
    {
        if (index >= this->children.size())
        {
            return NULL;
        }
        return this->children[index];
    }

    Patricia::Node *Patricia::Node::Insert(const std::string &Key, int Data)
    {
        if (this->key.length() < Key.length()) // If the key of the current node is shorter than the key of the node to insert
        {
            if (Key.starts_with(this->key)) // If the key of the node to insert starts with the key of the current node
            {
                for (auto child : this->children) // For each child of the current node
                {
                    return child->Insert(Key.substr(this->key.length(), Key.length() - this->key.length()), Data);
                }
                auto newChild = new Node(Key.substr(this->key.length(), Key.length() - this->key.length()), Data);
                this->children.push_back(newChild); // Add the new node as a child of the current node
                return newChild;                    // Return true because the operation succeeded
            }
            else
            {
                return nullptr; // Return false because the operation failed
            }
        }
        else if (this->key.length() > Key.length()) // If the key of the current node is longer than the key of the node to insert
        {
            if (this->key.starts_with(Key)) // If the key of the current node starts with the key of the node to insert
            {
                auto i = this->key.find(Key);                                // Find the index of the first character of the key of the node to insert in the key of the current node
                auto newChild = new Node(*this);                             // Create a new node with the same data and key as the current node
                newChild->key = this->key.substr(i, this->key.length() - i); // Set the key of the new node to the part of the key of the current node that is not in the key of the node to insert
                this->key = Key;                                             // Set the key of the current node to the key of the node to insert
                this->data = Data;                                           // Set the data of the current node to the data of the node to insert
                this->children.clear();                                      // Clear the children of the current node
                this->children.push_back(newChild);                          // Add the new node as a child of the current node
                return this;                                                 // Return true because the operation succeeded
            }
            else
            {
                return nullptr; // Return false because the operation failed
            }
        }
        else
        {
            if (this->key == Key) // If the key of the current node is the same as the key of the node to insert
            {
                this->data = Data; // Set the data of the current node to the data of the node to insert
                return this;       // Return true because the operation succeeded
            }
            else
            {
                return nullptr; // Return false because the operation failed
            }
        }
    }

    Patricia::Node *Patricia::Node::Search(const std::string &Key)
    {
        if (this->key == Key)
        {
            return this; // Return the current node because the key of the current node is the same as the key of the node to search for
        }
        else if (Key.starts_with(this->key)) // If the key of the node to search for starts with the key of the current node
        {
            for (auto child : this->children) // For each child of the current node
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

    void Patricia::Node::recursive_remove()
    {
        for (auto child : this->children)
        {
            child->recursive_remove();
        }

        // Remove the root
        delete this;
    }

    void Patricia::recursive_remove()
    {
        for (auto child : this->root)
        {
            child->recursive_remove(); // Remove the child
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
        for (auto child : root) // For each child of the root
        {
            child->recursive_remove(); // Remove the child
        }

        root.clear(); // Clear the root
    }

    size_t Patricia::Size()
    {
        return this->size; // Return the size
    }

    Patricia::Node *Patricia::Insert(const std::string &Key, int data)
    {
        std::string key = Key;
        for (auto &c : key)
            c = std::tolower(c);

        for (auto child : root) // For each child of the root
        {
            if (child->key[0] == key[0]) // If the first character of the key of the child is the same as the first character of the key of the node to search for
            {
                if (child->Insert(key, data)) // If the node was inserted
                {
                    return child; // Return the child
                }
            }
        }
        auto newNode = new Node(key, data); // Create a new node
        root.push_back(newNode);            // Add the new node as a child of the root
        return newNode;                     // Return the new node
    }

    int Patricia::Search(const std::string &Key)
    {
        std::string key = Key;
        for (auto &c : key)
            c = std::tolower(c);

        for (auto child : root) // For each child of the root
        {
            if (child->key[0] == key[0]) // If the first character of the key of the child is the same as the first character of the key of the node to search for
            {
                auto node = child->Search(key); // Search for the node
                if (node != nullptr)            // If the node was found
                {
                    return node->data; // Return the data of the node
                }
            }
        }
        return -1; // Return -1 because the node was not found
    }

    // bool Patricia::Delete(const std::string &Key)
    // {
    // }
}