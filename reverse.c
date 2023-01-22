#include <stdio.h>

void reverse(FILE *output, FILE *input) 
{
    char *text;
    long numbytes;

    fseek(input, 0L, SEEK_END);
    numbytes = ftell(input);
    fseek(input, 0L, SEEK_SET);

    text = (char *)calloc(numbytes, sizeof(char));
    if (text == NULL)
    {
        fprintf(stderr, "Malloc has failed.");
        exit(1);
    }

    fprintf(output, text);

    return;
}

int main(int argc, char const *argv[])
{
    FILE *input;
    FILE *output;

    if (argc != 2) 
    {
        fprintf(stderr, "reverse <input> <output>");
        exit(1);
    }

    input = fopen(argv[0], "r");

    if (input == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file 'input.txt'");
        exit(1);
    }

    output = fopen(argv[1], "r+");

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
