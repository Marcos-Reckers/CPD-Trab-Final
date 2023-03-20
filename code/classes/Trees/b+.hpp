#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>

namespace Trees
{
    const int MAX = 4096; // size of each node
    class BPlus
    {
    private:
        class Node
        {
        private:
            bool IS_LEAF;
            int *key, size;
            Node **ptr;
            friend class BPlus;

        public:
            Node();
        };

        Node *root;
        void insertInternal(int, Node *, Node *);
        void removeInternal(int, Node *, Node *);
        Node *findParent(Node *, Node *);

    public:
        BPlus();
        void search(int);
        void insert(int);
        void remove(int);
        void display(Node *);
        Node *getRoot();
        void cleanUp(Node *);
        ~BPlus();
    };
}

