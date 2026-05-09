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

    do
    {
        opt = getopt(argc, argv, "t:id:");
        switch (opt)
        {
        case 'i':
            return interactiveMain(argc, argv);

        case 't':
            txtfile = optarg;
            break;
        
        case 'd':
            while (optind < argc && argv[optind][0]!= '-')
            {
                dicts[dictc++] = argv[optind++];
            }
            break;

        default:
            print_usage(argv[0]);
            return 0;
        }
    } while (opt != -1);
    

    FILE *text = fopen(txtfile, "r");
    if (text == NULL)
    {
        printf("Could not open text file.\n");
        return 1;
    }

    node *root = load(dictc, dicts);
    if (root == NULL)
    {
        printf("Could not load dictionary.\n");
        fclose(text);
        return 2;
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
        
        if (!search(root, word))
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
