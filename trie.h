#ifndef TRIE
    #define TRIE

    #include <stdbool.h>

    typedef struct node node;
    
    node *load(int dictc, char **dicts);
    void unload (node *root);
    bool search(node *root, char *key);

#endif