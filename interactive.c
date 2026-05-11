#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "interactive.h"
#include "trie.h"

void print_usage(char *argv0)
{
    printf("Usage: %s -t <file> [-i] [-d <dict>]\n", argv0);
    printf("    -t <file>:   Specify the path of text file to spell-check\n");
    printf("    -i:          Enables interactive mode\n");
    printf("    -d <dict>:   Adds the specified dictionary to trie\n \n");
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
            dicts[dictc++] = argv[optind++];
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

    char *dest_file = "out.txt";
    FILE *dest = fopen(dest_file, "w");

    int c;
    char word_orig[64] = "";
    char word_lower[64] = "";
    int idx = 0;

    while ((c = fgetc(text)) != EOF)
    {
        if (isalpha((unsigned char)c) && idx < (int)sizeof(word_orig) - 1)
        {
            word_orig[idx] = (char)c;
            word_lower[idx] = (char)tolower((unsigned char)c);
            idx++;
        }
        else
        {
            if (idx > 0)
            {
                word_orig[idx] = '\0';
                word_lower[idx] = '\0';

                if (search(root, word_lower) || search(alts_root, word_lower))
                {
                    fprintf(dest, "%s", word_orig);
                    idx = 0;
                    fputc(c, dest);
                    continue;
                }

                char correction[46] = "";
                printf("Unknown word: \"%s\"\nEnter correction: ", word_orig);
                
                if (fgets(correction, sizeof(correction), stdin) != NULL)
                {
                    correction[strcspn(correction, "\n")] = '\0';
                }
                
                fprintf(dest, "%s", (correction[0] == '\0') ? word_orig : correction);

                idx = 0;
            }

            fputc(c, dest);
        }
    }

    if (idx > 0)
    {
        word_orig[idx] = '\0';
        word_lower[idx] = '\0';

        if (search(root, word_lower) || search(alts_root, word_lower))
        {
            fprintf(dest, "%s", word_orig);
            idx = 0;
        }
        else
        {
            char correction[46] = "";
            printf("Unknown word: \"%s\"\nEnter correction: ", word_orig);
            
            if (fgets(correction, sizeof(correction), stdin) != NULL)
            {
                correction[strcspn(correction, "\n")] = '\0';
            }
            
            fprintf(dest, "%s", (correction[0] == '\0') ? word_orig : correction);

            idx = 0;
        }
    }

    printf("Saved corrected text to %s\n", dest_file);

    unload(root);
    unload(alts_root);
    fclose(text);
    fclose(dest);

    
    return 0;
}
