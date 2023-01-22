// Code by William Joe Brashear
#include <stdio.h>
#include <stdlib.h>

struct LinkedList{
    char *text;
    struct LinkedList *next;
};

typedef struct LinkedList *ListNode;

void reverse(FILE *output, FILE *input) 
{
    char *text;
    long numbytes;
    size_t len = 0;

    while (getline(&text, &len, input) != -1) {
        fprintf(output, text);
    }

    return;
}

int main(int argc, char const *argv[])
{
    FILE *input;
    FILE *output;

    if (argc != 3) 
    {
        fprintf(stderr, "reverse <input> <output>");
        exit(1);
    }

    input = fopen(argv[1], "r");

    if (input == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file 'input.txt'");
        exit(1);
    }

    output = fopen(argv[2], "r+");

    if (output == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file 'output.txt'");
        fclose(input);
        exit(1);
    }

    reverse(output, input);
    
    fclose(input);
    fclose(output);

    return 0;
}
