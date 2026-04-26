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

void insert(node *root, char *word)
{
    node *ptr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (ptr->children[word[i] - 'a'] == NULL) 
        {
            node *nn = Trie();
            ptr->children[word[i] - 'a'] = nn;
        }

        ptr = ptr->children[word[i] - 'a'];
    }
    ptr->isWordEnd = true;
}

node *load()
{
    FILE *dict = fopen("dict1.txt", "r");

    if (dict == NULL)
    {
        printf("could not open dictionary.\n");
        return NULL;
    }
    
    node *root = Trie();

    if (root == NULL)
    {
        return NULL;
    }
    

    char word[45] = "";
    while (fgets(word, 45, dict))
    {
        word[strcspn(word, "\n")] = '\0';
        insert(root, word);
    }

    fclose(dict);

    return root;
}

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
    
    if (ptr->isWordEnd)
    {
        return true;
    }
    return false;
}
