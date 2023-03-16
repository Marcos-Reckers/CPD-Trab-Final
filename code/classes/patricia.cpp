#include "patricia.hpp"

namespace Trees
{
    //----------------------------------------------------------------------------
    Patricia::Node::Node()
        : data(-1), bit_index(-1), key("")
    {
        this->left = this;
        this->right = this;
    }

    //----------------------------------------------------------------------------

    Patricia::Node::Node(const std::string &key, int data, int bit_index, Node *left, Node *right)
        : data(data), bit_index(bit_index), key(key)
    {
        this->left = left;
        this->right = right;
    }

    //----------------------------------------------------------------------------
    Patricia::Node::~Node()
    {
    }

    //----------------------------------------------------------------------------
    int Patricia::Node::GetData()
    {
        return this->data;
    }

    //----------------------------------------------------------------------------
    bool Patricia::Node::SetData(int data)
    {
        this->data = data;
        return true;
    }

    //----------------------------------------------------------------------------
    std::string Patricia::Node::GetKey()
    {
        return this->key;
    }

    //----------------------------------------------------------------------------
    Patricia::Node *Patricia::Node::GetLeft()
    {
        return this->left;
    }

    //----------------------------------------------------------------------------
    Patricia::Node *Patricia::Node::GetRight()
    {
        return this->right;
    }

    //----------------------------------------------------------------------------
    Patricia::Patricia()
    {
        // Create the root of the structure. The root is never moved
        // around in the trie (i.e. it always stays at the top of the structure).
        // This prevents further complications having to do with node removal.
        root = new Node();
        root->key = (char *)calloc(256, 1);
    }

    //----------------------------------------------------------------------------
    Patricia::~Patricia()
    {
        recursive_remove(root);
    }

    //----------------------------------------------------------------------------
    Patricia::Node *Patricia::Insert(const std::string &key, int data)
    {

        Node *parent, *tree, *place;

        // Start at the root
        parent = root;
        tree = parent->right;

        // Navigate down the tree and look for the key
        while (parent->bit_index < tree->bit_index)
        {
            parent = tree;
            tree = bit_get(key, tree->bit_index) ? tree->right : tree->left;
        }

        // Is the key already in the tree?
        if (key == tree->key)
            return NULL; // Already in the tree!

        // Find the first bit that does not match.
        int i = bit_first_different(key, tree->key);

        // Find the appropriate place in the tree where
        // the node has to be inserted
        parent = root;
        place = parent->right;
        while ((parent->bit_index < place->bit_index) && (place->bit_index < i))
        {
            parent = place;
            place = bit_get(key, place->bit_index) ? place->right : place->left;
        }

        // Allocate a new node and initialize it.
        tree = new Node(key, data, i, (bit_get(key, i) ? place : tree), (bit_get(key, i) ? tree : place));

        // Rewire
        if (bit_get(key, parent->bit_index))
            parent->right = tree;
        else
            parent->left = tree;

        // Return the newly created node
        return tree;
    }

    //----------------------------------------------------------------------------

    int Patricia::Search(const std::string &key)
    {

        // Lookup the node
        Node *node = SearchNode(key);

        // Failed?
        if (!node)
            return -1;

        // Return the data stored in this node
        return node->data;
    }

    //----------------------------------------------------------------------------
    Patricia::Node *Patricia::SearchNode(const std::string &key)
    {

        Node *parent;
        Node *place;

        // Start at the root.
        parent = root;
        place = root->right;

        // Go down the Patricia structure until an upward
        // link is encountered.
        while (parent->bit_index < place->bit_index)
        {
            parent = place;
            place = bit_get(key, place->bit_index) ? place->right : place->left;
        }

        // Perform a full string comparison, and return NULL if
        // the key is not found at this location in the structure.
        if (!(key == place->key))
            return NULL;

        // Return the node
        return place;
    }

    //----------------------------------------------------------------------------
    // bool Patricia::Delete(const std::string &Key)
    // {
    //     std::string key = Key;
    //     Node *parent, *tree, *place, *grandparent, *lastparent;
    //     int bitparent, bitleft, bitright;
    //     char *key = NULL;

    //     // Start at the root
    //     parent = root;
    //     tree = (Node *)(parent->right);

    //     // Navigate down the tree and look for the key
    //     while (parent->bit_index < tree->bit_index)
    //     {
    //         grandparent = parent;
    //         parent = tree;
    //         tree = (Node *)(bit_get(key, tree->bit_index) ? tree->right : tree->left);
    //     }

    //     // Is the key in the tree? If not, get out!
    //     if (!(key == tree->key))
    //         return false; // The key could not be found!

    //     // Copy p's key to t
    //     if (tree != parent)
    //         tree->key = parent->key;

    //     // Is p a leaf?
    //     bitparent = parent->bit_index;
    //     bitleft = ((Node *)(parent->left))->bit_index;
    //     bitright = ((Node *)(parent->right))->bit_index;

    //     if ((bitleft > bitparent) || (bitright > bitparent))
    //     {

    //         // There is at least one downward edge.

    //         if (parent != tree)
    //         {

    //             // Look for a new (intermediate) key
    //             key = parent->key;

    //             lastparent = parent;
    //             place = (Node *)(bit_get(key, parent->bit_index) ? parent->right : parent->left);

    //             while (lastparent->bit_index < place->bit_index)
    //             {
    //                 lastparent = place;
    //                 place = (Node *)(bit_get(key, place->bit_index) ? place->right : place->left);
    //             }

    //             // If the intermediate key was not found, we have a problem..
    //             if (!(key == place->key))
    //             {
    //                 return false; // The key could not be found!
    //             }

    //             // Rewire the leaf (lp) to point to t
    //             if (bit_get(key, lastparent->bit_index))
    //                 lastparent->right = tree;
    //             else
    //                 lastparent->left = tree;
    //         }

    //         // Rewire the parent to point to the real child of p
    //         if (grandparent != parent)
    //         {
    //             Node *child = (Node *)(bit_get(key, parent->bit_index) ? parent->left : parent->right);
    //             if (bit_get(key, grandparent->bit_index))
    //                 grandparent->right = child;
    //             else
    //                 grandparent->left = child;
    //         }
    //     }
    //     else
    //     {

    //         // Both edges (left, right) are pointing upwards or to the node (self-edges).

    //         // Rewire the parent
    //         if (grandparent != parent)
    //         {
    //             Node *blx = (Node *)(parent->left);
    //             Node *brx = (Node *)(parent->right);
    //             if (bit_get(key, grandparent->bit_index))
    //                 grandparent->right = (((blx == brx) && (blx == parent)) ? grandparent : ((blx == parent) ? brx : blx));
    //             else
    //                 grandparent->left = (((blx == brx) && (blx == parent)) ? grandparent : ((blx == parent) ? brx : blx));
    //         }
    //     }

    //     // Deallocate p (no longer needed)
    //     delete parent;

    //     // Success!
    //     return true;
    // }

    //--------------------------------------------------------------------------
    void Patricia::recursive_remove(Patricia::Node *root)
    {

        Node *left = (Node *)root->left;
        Node *right = (Node *)root->right;

        // Remove the left branch
        if ((left->bit_index >= root->bit_index) && (left != root) && (left != root))
            recursive_remove(left);

        // Remove the right branch
        if ((right->bit_index >= root->bit_index) && (right != root) && (right != root))
            recursive_remove(right);

        // Remove the root
        delete root;
    }

    //----------------------------------------------------------------------------

    int Patricia::bit_get(const std::string &key, int n)
    {
        if (n < 0)
            return 2; // "pseudo-bit" with a value of 2.
        int k = (n & 0x7);
        return ((*(key.c_str() + (n >> 3))) >> k) & 0x1;
    }
    //----------------------------------------------------------------------------

    int Patricia::bit_first_different(const std::string &k1, const std::string &k2)
    {
        if (!k1.c_str() || !k2.c_str())
            return 0; // First bit is different!

        int n = 0;
        int d = 0;
        while ((k1[n] == k2[n]) && (k1[n] != 0) && (k2[n] != 0))
            n++;
        while (bit_get(&k1[n], d) == bit_get(&k2[n], d))
            d++;
        return ((n << 3) + d);
    }
}