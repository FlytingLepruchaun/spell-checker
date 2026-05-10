#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "trie.h"
#include "interactive.h"

void strlwr(char *str);

int main(int argc, char *const argv[])
{
    int opt, dictc = 0;
    char *txtfile = NULL;
    char *dicts[10] = {NULL};

    while ((opt = getopt(argc, argv, "t:id:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            return interactiveMain(argc, argv);

        case 't':
            txtfile = optarg;
        break;
        
        case 'd':
            dicts[dictc++] = optarg;
        break;

        default:
            print_usage(argv[0]);
            return 0;
        }
    }    

    if (txtfile == NULL)
    {
        print_usage(argv[0]);
        return 0;
    }

    FILE *text = fopen(txtfile, "r");
    if (text == NULL)
    {
        printf("Could not open text file.\n");
        return 1;
    }

    node *root = load(1, (char *[]){"dicts/dictionary-small.txt"});
    if (root == NULL)
    {
        printf("Could not load dictionary.\n");
        fclose(text);
        return 2;
    }

    node *alts_root = NULL;
    if (dictc != 0)
    {
        alts_root = load(dictc, dicts);
        if (alts_root == NULL)
        {
            printf("Could not load auxilary dictionaries.\n");
            return 2;
        }
    }
    
    unsigned int misspelt = 0;
    char word[25] = "";
    printf("MISSPELT WORDS: \n");
    while(fscanf(text, "%24s", word) == 1)
    {
        unsigned int len = strlen(word);
        while(len > 0 && ispunct((unsigned char)word[len - 1]))
        {
            word[--len] = '\0';
        }
        if (word[0] == '\0')
        {
            continue;   
        } 
        strlwr(word);
        
        if (!search(root, word) && (alts_root == NULL || !search(alts_root, word)))
        {
            printf("%s\n", word);
            ++misspelt;
        }
    }

    printf("\nTOTAL MISSPELT WORDS: %u\n", misspelt);
    
    unload(root);
    fclose(text);
    return 0;
}

void strlwr(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}
