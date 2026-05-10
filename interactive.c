#include <stdio.h>
#include <getopt.h>

#include "interactive.h"
#include "trie.h"

void fcopy(FILE *dest, FILE *src);

void print_usage(char *argv0)
{
    printf("Usage: %s -t <file> [-i] [-d <dicts>...]\n", argv0);
    printf("    -t <file>:   Specify the path of text file to spell-check\n");
    printf("    -i:          Enables interactive mode\n");
    printf("    -d <dicts>:  Adds all dictionaries in <dicts> list to trie\n \n");
    printf("Running this script without any option or with missing options shows this help\n");
}

int interactiveMain(int argc, char *const argv[]) 
{
    int opt, dictc = 0;
    char *txtfile = NULL;
    char *dicts[10] = {NULL};

    optind = 1;
    while ((opt = getopt(argc, argv, "t:d:i")) != -1)
    {
        switch (opt)
        {
        case 't':
            txtfile = optarg;
        break;
        
        case 'd':
            while (optind < argc && argv[optind][0]!= '-')
            {
                dicts[dictc++] = argv[optind++];
            }
        break;

        case 'i':
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

    node *root = load();
    if (root == NULL)
    {
        printf("Could not load dictionary.\n");
        fclose(text);
        return 2;
    }

    node *alts_root = NULL;
    if (dictc != 0)
    {
        alts_root = load();
        if (alts_root == NULL)
        {
            printf("Could not load auxilary dictionaries.\n");
            return 2;
        }
        
    }

    char *dest_file = "out.txt";
    FILE *dest = fopen(dest_file, "w");
    fcopy(dest, text);
    
    char word[45] = "";
    while (fscanf(text, "%45s", word) == 1)
    {
        
        if (!search(root, word) || !search(alts_root, word))
        {
            
        }
    }
    
    return 0;
}

void fcopy(FILE *dest, FILE *src)
{
    char ch;
    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, dest);
    }
}