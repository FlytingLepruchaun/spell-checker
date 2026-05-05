#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

void strlwr(char *str);
void interactive(node *root, FILE *text);

int cleanWord(char word[25]);

int main(int argc, char const *argv[])
{

    bool isInteractive = true;

    printf("Enter adress of text file: ");
    char txtfile[256];
    scanf("%255s", txtfile);

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
    
    if (isInteractive)
    {
        interactive(root, text);
    }
    

    unsigned int misspelt = 0;
    char word[25] = "";
    printf("MISSPELT WORDS: \n");
    while(fscanf(text, "%24s", word) == 1)
    {
        if (cleanWord(word))
            continue;
        
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

void interactive(node *root, FILE *text)
{
    char word[25] = "", cpy[45] = "";
    FILE *fixed = fopen("fixed.txt", "w");
    int loc = 0;
    while(fscanf(text, "%24s", word) == 1)
    {
        strcpy(cpy, word);
        if (cleanWord(word))
            continue;
        if (!search(root, word))
        {
            printf("%s\n", cpy);
            printf("Enter correct spelling: ");
            scanf("%24s", cpy);
        }
        strcat(cpy, " ");
        fprintf(fixed, "%s", cpy);
    }
}

int cleanWord(char word[25])
{
    unsigned int len = strlen(word);
    while (len > 0 && ispunct((unsigned char)word[len - 1]))
    {
        word[--len] = '\0';
    }
    if (word[0] == '\0')
    {
        {
            return 1;
        };
    }
    strlwr(word);
    return 0;
}
