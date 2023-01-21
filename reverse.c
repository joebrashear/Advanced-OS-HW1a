#include <stdio.h>

void reverse() 
{

}

int main(int argc, char const *argv[])
{
    if (argc != 2) 
    {
        fprintf(stderr, "reverse <input> <output>");
        exit(1);
    }

    FILE *input = fopen(argv[0], "r");

    if (input == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file 'input.txt'");
        exit(1);
    }

    FILE *output = fopen(argv[1], "r+");

    if (output == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file 'output.txt'");
        fclose(input);
        exit(1);
    }

    return 0;
}
