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
            int data = -1;
            int bit_index = -1;
            friend class Patricia;
            std::string key;
            Node *left;
            Node *right;

        public:
            Node();
            Node(const std::string &key, int data, int bit_index, Node *left, Node *right);
            virtual ~Node();

            int GetData();
            bool SetData(int data);

            std::string GetKey();

            Node *GetLeft();
            Node *GetRight();
        };
        void recursive_remove(Node *);
        int bit_get(const std::string &, int);
        int bit_first_different(const std::string &, const std::string &);
        Node *root;

    public:
        Patricia();
        virtual ~Patricia();

        // Purpose:	Insert a new key+data pair in the Patricia structure, and
        //          return the new node.
        virtual Node *Insert(const std::string &key, int data);

        // Purpose:	Search for the given key, and return the data associated
        //          with it (or NULL).
        virtual int Search(const std::string &key);

        // Purpose:	Search for the given key, and return the node that
        //          contains it (or NULL).
        virtual Node *SearchNode(const std::string &key);

        // Purpose:	Remove the node containing the given key. Return
        //          true if the operation succeeded, false otherwise.
        // virtual bool Delete(const std::string &key);
    };
}