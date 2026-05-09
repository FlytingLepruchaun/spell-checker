#ifndef TRIE
    #define TRIE

    #include <stdbool.h>

    typedef struct node node;
    
    node *load();
    void unload (node *root);
    bool search(node *root, char *key);

#endif