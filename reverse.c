// Code by William Joe Brashear
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int debug = 0;

typedef struct LinkedList{
    char *text;
    struct LinkedList *next;
} ListNode;

int is_same(FILE *input, FILE *output)
{
    char *input_text = NULL;
    size_t input_len = 0;
    char *output_text = NULL;
    size_t output_len = 0;
    int is_input_end = 0;
    int is_output_end = 0;

    while (is_input_end != -1 && is_output_end != -1) 
    {
        is_input_end = getline(&input_text, &input_len, input);
        is_output_end = getline(&output_text, &output_len, output);
        
        if (strcmp(input_text, output_text) != 1) 
        {
            return 0;
        }
    }

    fprintf(stderr, "Input and output file must differ\n");
    return 1;
}

void reverse_to_output(ListNode *head, FILE *output)
{
    ListNode *temp = NULL;

    // add to output and deallocate LL
    while (head != NULL)
    {
        fprintf(output, "%s", head->text);
        temp = head;
        head = head->next;
        free(temp);
    }

    return;
}

ListNode* reverse_ll(ListNode *head) 
{
    ListNode *prev = NULL;
    ListNode *curr = head;

    while (curr != NULL)
    {
        ListNode *temp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = temp;
    }

    return prev;
}



void deallocate_List(ListNode *head) 
{
    ListNode *temp = NULL;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }

    return;
}

int read_text(char *text, size_t len, FILE *input, ListNode *node, ListNode *head)
{
    
    while (getline(&text, &len, input) != -1) 
    {
        
        // Store line in Linked List
        node->next = (ListNode*)calloc(1, sizeof(ListNode));

        if (node->next == NULL) 
        {
            fprintf(stderr, "Malloc has failed\n");
            free(text);
            deallocate_List(head);
            return 1;
        }

        node->next->text = (char*)calloc(len, sizeof(char));

        if (node->next->text == NULL)
        {
            fprintf(stderr, "Malloc has failed\n");
            free(text);
            deallocate_List(head);
            return 1;
        }
        
        strcpy(node->next->text, text);

        if (debug == 1) {
            printf("%s", text);
            printf("copy: %s", node->next->text);
        }

        node = node->next;
    }

    return 0;
}

int reverse(FILE *output, FILE *input) 
{
    char *text = NULL;
    size_t len = 0;
    
    ListNode *node = (ListNode*)calloc(1, sizeof(ListNode));

    if (node == NULL) 
    {
        fprintf(stderr, "Malloc has failed\n");
        return 1;
    }

    ListNode *head = node;

    if (read_text(text, len, input, node, head) == 1)
    {
        return 1;
    }

    // free dummy head
    ListNode *temp = head;
    head = head->next;
    free(temp);
    
    head = reverse_ll(head);
    reverse_to_output(head, output);

    return 0;
}

int main(int argc, char const *argv[])
{
    FILE *input = NULL;
    FILE *output = NULL;
    struct stat input_stat;
    int input_stat_ret = 0;
    struct stat output_stat;
    int output_stat_ret = 0;

    if (argc > 3 || argc < 1) 
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 1)
        input = fopen(argv[1], "r");
    else
    {
        char *file_name = NULL;
        scanf("%s", file_name);
        input = fopen(file_name, "r");
    }

    if (input == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
        exit(1);
    }

    if (argc == 3) 
    {
        output = fopen(argv[2], "w+");

        if (output == NULL) 
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            fclose(input);
            exit(1);
        }

        input_stat_ret = stat(argv[1], &input_stat);

        if (input_stat_ret < 0)
        {
            fprintf(stderr, "Cannot get input file stat\n");
            fclose(input);
            fclose(output);
            exit(1);
        }

        output_stat_ret = stat(argv[2], &output_stat);

        if (output_stat_ret < 0)
        {
            fprintf(stderr, "Cannot get output file stat\n");
            fclose(input);
            fclose(output);
            exit(1);
        }

        if (input_stat.st_ino == output_stat.st_ino)
        {
            fprintf(stderr, "reverse: input and output file must differ\n");
            fclose(input);
            fclose(output);
            exit(1);
        }
    }
    else
    {
        output = stdout;
    }
    
    if (reverse(output, input) == 1) 
    {
        fclose(input);
        fclose(output);
        exit(1);
    }
    
    fclose(input);
    fclose(output);

    return 0;
}
