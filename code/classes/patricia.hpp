
namespace Trees
{
    class PatriciaTree
    {
        public:
            PatriciaTree();
            ~PatriciaTree();
            void insert(int key, int value);
            void remove(int key);
            int search(int key);
            void print();
        private:
            struct Node
            {
                int key;
                int value;
                Node* left;
                Node* right;
            };
            Node* root;
    };
}