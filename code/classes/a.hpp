// #pragma once
// #include <iostream>
// #include <string>
// #include <vector>

// #include <stdlib.h>
// #include <string.h>
// namespace Tree
// {
//     class PatriciaTrieNode
//     {
//     private:
//         friend class PatriciaTrie;
//         int bit_index;
//         std::string key;
//         int data;
//         PatriciaTrieNode *left;
//         PatriciaTrieNode *right;

//     public:
//         PatriciaTrieNode();
//         PatriciaTrieNode(const std::string &key, int data, int bit_index, PatriciaTrieNode *left, PatriciaTrieNode *right);
//         virtual ~PatriciaTrieNode();

//         int GetData();
//         bool SetData(int data);

//         std::string GetKey();

//         PatriciaTrieNode *GetLeft();
//         PatriciaTrieNode *GetRight();
//     };

//     class PatriciaTrie
//     {
//     private:
//         void recursive_remove(PatriciaTrieNode *);
//         int bit_get(const std::string &, int);
//         int bit_first_different(const std::string &, const std::string &);
//         PatriciaTrieNode *root;

//     public:
//         PatriciaTrie();
//         virtual ~PatriciaTrie();

//         // Purpose:	Insert a new key+data pair in the Patricia structure, and
//         //          return the new node.
//         virtual PatriciaTrieNode *Insert(const std::string &key, int data);

//         // Purpose:	Search for the given key, and return the data associated
//         //          with it (or NULL).
//         virtual int Lookup(const std::string &key);

//         // Purpose:	Search for the given key, and return the node that
//         //          contains it (or NULL).
//         virtual PatriciaTrieNode *LookupNode(const std::string &key);

//         // Purpose:	Remove the node containing the given key. Return
//         //          true if the operation succeeded, false otherwise.
//         virtual bool Delete(const std::string &key);
//     };

//     //----------------------------------------------------------------------------
//     PatriciaTrieNode::PatriciaTrieNode()
//     : key(key), data(data), bit_index(bit_index)
//     {
//         this->left = this;
//         this->right = this;
//     }

//     //----------------------------------------------------------------------------

//     PatriciaTrieNode::PatriciaTrieNode(const std::string &key, int data, int bit_index, PatriciaTrieNode *left, PatriciaTrieNode *right)
//     : key(key), data(data), bit_index(bit_index)
//     {
//         this->left = left;
//         this->right = right;
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrieNode::~PatriciaTrieNode()
//     {
//     }

//     //----------------------------------------------------------------------------
//     int PatriciaTrieNode::GetData()
//     {
//         return this->data;
//     }

//     //----------------------------------------------------------------------------
//     bool PatriciaTrieNode::SetData(int data)
//     {
//         this->data = data;
//         return true;
//     }

//     //----------------------------------------------------------------------------
//     std::string PatriciaTrieNode::GetKey()
//     {
//         return this->key;
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrieNode *PatriciaTrieNode::GetLeft()
//     {
//         return this->left;
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrieNode *PatriciaTrieNode::GetRight()
//     {
//         return this->right;
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrie::PatriciaTrie()
//     {
//         // Create the root of the structure. The root is never moved
//         // around in the trie (i.e. it always stays at the top of the structure).
//         // This prevents further complications having to do with node removal.
//         root = new PatriciaTrieNode();
//         root->key = (char *)calloc(256, 1);
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrie::~PatriciaTrie()
//     {
//         recursive_remove(root);
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrieNode *PatriciaTrie::Insert(const std::string &key, int data)
//     {

//         PatriciaTrieNode *parent, *tree, *place;

//         // Start at the root
//         parent = root;
//         tree = (PatriciaTrieNode *)(parent->right);

//         // Navigate down the tree and look for the key
//         while (parent->bit_index < tree->bit_index)
//         {
//             parent = tree;
//             tree = (PatriciaTrieNode *)(bit_get(key, tree->bit_index) ? tree->right : tree->left);
//         }

//         // Is the key already in the tree?
//         if (key == tree->key)
//             return NULL; // Already in the tree!

//         // Find the first bit that does not match.
//         int i = bit_first_different(key, tree->key);

//         // Find the appropriate place in the tree where
//         // the node has to be inserted
//         parent = root;
//         place = (PatriciaTrieNode *)(parent->right);
//         while ((parent->bit_index < place->bit_index) && (place->bit_index < i))
//         {
//             parent = place;
//             place = (PatriciaTrieNode *)(bit_get(key, place->bit_index) ? place->right : place->left);
//         }

//         // Allocate a new node and initialize it.
//         tree = new PatriciaTrieNode(key, data, i, (bit_get(key, i) ? place : tree), (bit_get(key, i) ? tree : place));

//         // Rewire
//         if (bit_get(key, parent->bit_index))
//             parent->right = tree;
//         else
//             parent->left = tree;

//         // Return the newly created node
//         return tree;
//     }

//     //----------------------------------------------------------------------------
    
//     int PatriciaTrie::Lookup(const std::string &key)
//     {

//         // Lookup the node
//         PatriciaTrieNode *node = LookupNode(key);

//         // Failed?
//         if (!node)
//             return NULL;

//         // Return the data stored in this node
//         return node->data;
//     }

//     //----------------------------------------------------------------------------
//     PatriciaTrieNode *PatriciaTrie::LookupNode(const std::string &key)
//     {

//         PatriciaTrieNode *parent;
//         PatriciaTrieNode *place;

//         // Start at the root.
//         parent = root;
//         place = (PatriciaTrieNode *)(root->right);

//         // Go down the Patricia structure until an upward
//         // link is encountered.
//         while (parent->bit_index < place->bit_index)
//         {
//             parent = place;
//             place = (PatriciaTrieNode *)(bit_get(key, place->bit_index) ? place->right : place->left);
//         }

//         // Perform a full string comparison, and return NULL if
//         // the key is not found at this location in the structure.
//         if (!(key == place->key))
//             return NULL;

//         // Return the node
//         return place;
//     }

//     //----------------------------------------------------------------------------
//     bool PatriciaTrie::Delete(std::string k)
//     {

//         PatriciaTrieNode *p, *t, *x, *pp, *lp;
//         int bp, bl, br;
//         char *key = NULL;

//         // Start at the root
//         p = root;
//         t = (PatriciaTrieNode *)(p->right);

//         // Navigate down the tree and look for the key
//         while (p->bit_index < t->bit_index)
//         {
//             pp = p;
//             p = t;
//             t = (PatriciaTrieNode *)(bit_get(k, t->bit_index) ? t->right : t->left);
//         }

//         // Is the key in the tree? If not, get out!
//         if (!key_compare(k, t->key))
//             return false; // The key could not be found!

//         // Copy p's key to t
//         if (t != p)
//             key_copy(p, t);

//         // Is p a leaf?
//         bp = p->bit_index;
//         bl = ((PatriciaTrieNode *)(p->left))->bit_index;
//         br = ((PatriciaTrieNode *)(p->right))->bit_index;

//         if ((bl > bp) || (br > bp))
//         {

//             // There is at least one downward edge.

//             if (p != t)
//             {

//                 // Look for a new (intermediate) key
//                 key = strdup(p->key);

//                 lp = p;
//                 x = (PatriciaTrieNode *)(bit_get(key, p->bit_index) ? p->right : p->left);

//                 while (lp->bit_index < x->bit_index)
//                 {
//                     lp = x;
//                     x = (PatriciaTrieNode *)(bit_get(key, x->bit_index) ? x->right : x->left);
//                 }

//                 // If the intermediate key was not found, we have a problem..
//                 if (!key_compare(key, x->key))
//                 {
//                     free(key);
//                     return false; // The key could not be found!
//                 }

//                 // Rewire the leaf (lp) to point to t
//                 if (bit_get(key, lp->bit_index))
//                     lp->right = t;
//                 else
//                     lp->left = t;
//             }

//             // Rewire the parent to point to the real child of p
//             if (pp != p)
//             {
//                 PatriciaTrieNode *ch = (PatriciaTrieNode *)(bit_get(k, p->bit_index) ? p->left : p->right);
//                 if (bit_get(k, pp->bit_index))
//                     pp->right = ch;
//                 else
//                     pp->left = ch;
//             }

//             // We no longer need 'key'
//             free(key);
//             key = NULL;
//         }
//         else
//         {

//             // Both edges (left, right) are pointing upwards or to the node (self-edges).

//             // Rewire the parent
//             if (pp != p)
//             {
//                 PatriciaTrieNode *blx = (PatriciaTrieNode *)(p->left);
//                 PatriciaTrieNode *brx = (PatriciaTrieNode *)(p->right);
//                 if (bit_get(k, pp->bit_index))
//                     pp->right = (((blx == brx) && (blx == p)) ? pp : ((blx == p) ? brx : blx));
//                 else
//                     pp->left = (((blx == brx) && (blx == p)) ? pp : ((blx == p) ? brx : blx));
//             }
//         }

//         // Deallocate p (no longer needed)
//         delete p;

//         // Success!
//         return true;
//     }

//     //--------------------------------------------------------------------------
//     void PatriciaTrie::recursive_remove(PatriciaTrieNode *root)
//     {

//         PatriciaTrieNode *l = (PatriciaTrieNode *)root->left;
//         PatriciaTrieNode *r = (PatriciaTrieNode *)root->right;

//         // Remove the left branch
//         if ((l->bit_index >= root->bit_index) && (l != root) && (l != root))
//             recursive_remove(l);

//         // Remove the right branch
//         if ((r->bit_index >= root->bit_index) && (r != root) && (r != root))
//             recursive_remove(r);

//         // Remove the root
//         delete root;
//     }

//     //----------------------------------------------------------------------------
    
//     int PatriciaTrie::bit_get(std::string bit_stream, int n)
//     {
//         if (n < 0)
//             return 2; // "pseudo-bit" with a value of 2.
//         int k = (n & 0x7);
//         return ((*(bit_stream + (n >> 3))) >> k) & 0x1;
//     }
//     //----------------------------------------------------------------------------
    
//     int PatriciaTrie::bit_first_different(std::string k1, std::string k2)
//     {
//         if (!k1 || !k2)
//             return 0; // First bit is different!
//         int n = 0;
//         int d = 0;
//         while ((k1[n] == k2[n]) &&
//                (k1[n] != 0) &&
//                (k2[n] != 0))
//             n++;
//         while (bit_get(&k1[n], d) == bit_get(&k2[n], d))
//             d++;
//         return ((n << 3) + d);
//     }
// }