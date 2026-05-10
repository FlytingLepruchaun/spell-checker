#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

struct node
{
    struct node *children[26];
    bool isWordEnd;
};

/* 
    Function to allocate memory for a node and initialize all members of the node to 0 or equivalent.
*/
node *Trie(void)
{
    node *nn = malloc(sizeof(node));
    if (nn == NULL)
    {
        printf("malloc failed.\n");
        return NULL;
    }
    
    for (int i = 0; i < 26; i++)
    {
        nn->children[i] = NULL;
    }
    nn->isWordEnd = false;

    return nn;
}

/*
    inserts one word into the trie 

    for every character in the word:
        - calculate index using (word[i] - 'a')
        - check if a child node exists at that index
        - if not, create a new node using Trie() and link it
        - move the pointer to the child node

    after processing all characters, marks the final node as end of a valid word

    error codes:
        0 - success
        1 - Trie() failed / memory allocation error
*/
int insert(node *root, char *word)
{
    node *ptr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (ptr->children[word[i] - 'a'] == NULL) 
        {
            node *nn = Trie();
            if (nn == NULL)
            {
                printf("insertion failed.\n");
                return 1;
            }
            
            ptr->children[word[i] - 'a'] = nn;
        }

        ptr = ptr->children[word[i] - 'a'];
    }
    ptr->isWordEnd = true;

    return 0;
}

/*
    function to load dictionary into trie
    returns root node address during normal functioning

    else returns NULL if: dict could not be opened, root could not be initialized, inset function fails
*/
node *load()
{
    FILE *dict = fopen("dicts/dictionary-small.txt", "r");
    if (dict == NULL)
    {
        printf("could not open dictionary.\n");
        return NULL;
    }
    
    node *root = Trie();
    if (root == NULL)
    {
        printf("could not initialize trie.\n");
        fclose(dict);
        return NULL;
    }
    

    char word[24] = "";
    while (fgets(word, 24, dict))
    {
        word[strcspn(word, "\n")] = '\0';
        if (insert(root, word))
        {
            printf("could not load dictionary.\n");
            unload(root);
            fclose(dict);
            return NULL;
        }
    }

    fclose(dict);
    return root;
}

/*
    recursively frees all nodes in the trie
    traverses children first (post-order), then frees the node
*/
void unload (node *root)
{

    if (root == NULL)
    {
        return;
    }
    
    for (int i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
        {
            unload(root->children[i]);
        }
    }

    free(root);
}

/*
    search for a word in the trie

    traverse nodes using each character in key.
    if any required child is missing, return false.
    after traversal, return true only if node marks end of word.
*/
bool search(node *root, char *key)
{
    if (root == NULL)
    {
        return false;
    }
    
    node *ptr = root;
    
    for (int i = 0; key[i] != '\0'; i++) 
    {
        int index = key[i] - 'a';
        if (ptr->children[index] == NULL)
        {
            return false;
        }
    
        ptr = ptr->children[index];
    }
    
    return ptr->isWordEnd;
}
