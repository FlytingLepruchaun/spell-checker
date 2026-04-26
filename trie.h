#ifndef TRIE
    #define TRIE
    typedef struct node node;
    
    node *load();
    void unload (node *root);
    bool search(node *root, char *key);

#endif