#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace Trees
{
    class Patricia
    {
    private:
        class Node
        {
        private:
            friend class Patricia;        // Allow Patricia to access private members of Node
            int data = -1;                // -1 means no data
            std::string key;              // Key of the node
            std::vector<Node *> children; // Children of the node

        public:
            Node();                                                   // Default constructor
            Node(const std::string &Key, int Data);                   // Constructor with key and data
            Node(const Node &node);                                   // Copy constructor
            ~Node();                                                  // Destructor
            std::string GetKey();                                     // Get the key of the node
            int GetData();                                            // Get the data of the node
            bool SetData(int data);                                   // Set the data of the node
            Node *GetChild(size_t index);                             // Get the child of the node at the given index
            Patricia::Node *Insert(const std::string &Key, int Data); // Insert a new node in the children of the current node
            Patricia::Node *Search(const std::string &Key);           // Search for a node in the children of the current node
            void recursive_remove();                                  // Remove all the children of the current node
            friend std::ostream &operator<<(std::ostream &os, const Node &n)
            {
                os << "Key: " << n.key << " Data: " << n.data;
                return os;
            }
            void print(std::ostream &os, size_t i = 0)
            {
                for (size_t j = 0; j < i; j++)
                    os << "    ";
                os << *this << std::endl;
                for (auto child : children)
                {
                    child->print(os, i + 1);
                }
            }
        };

        void recursive_remove();  // Remove all the nodes of the Patricia structure
        size_t size = 0;          // Number of nodes in the Patricia structure
        std::vector<Node *> root; // Root of the Patricia structure

    public:
        Patricia();                                     // Default constructor
        ~Patricia();                                    // Destructor
        size_t Size();                                  // Get the number of nodes in the Patricia structure
        Node *Insert(const std::string &key, int data); // Insert a new node in the Patricia structure
        int Search(const std::string &key);             // Search for a node in the Patricia structure and return its data (or -1 if not found)
        // bool Delete(const std::string &key); // Remove a node from the Patricia structure
        friend std::ostream &operator<<(std::ostream &os, const Patricia &tree)
        {
            for (auto node : tree.root)
            {
                node->print(os);
            }
            return os;
        }
    };
}