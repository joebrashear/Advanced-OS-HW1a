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

void reverse_to_output(ListNode *head, FILE *output)
{
    ListNode *temp = NULL;

    // add to output and deallocate LL
    while (head != NULL)
    {
        fprintf(output, "%s", head->text);
        temp = head;
        head = head->next;
        free(temp->text);
        free(temp);
    }

    return;
}

void deallocate_List(ListNode *head) 
{
    ListNode *temp = NULL;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->text);
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

        node = node->next;
    }

    free(text);

    return 0;
}

int reverse(FILE *output, FILE *input) 
{
    char *text = NULL;
    size_t len = 0;
    
    ListNode *node = (ListNode*)calloc(1, sizeof(ListNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Malloc has failed for node\n");
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
    struct stat *input_stat = NULL;
    int input_stat_ret = 0;
    struct stat *output_stat = NULL;
    int output_stat_ret = 0;

    switch(argc)
    {
        case 1:
                input = stdin;
                output = stdout;
                break;
        case 2:
                input = fopen(argv[1], "r");
                if (input == NULL) 
                {
                    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
                    exit(1);
                }
                output = stdout;
                break;
        case 3:
                input = fopen(argv[1], "r");
                if (input == NULL) 
                {
                    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
                    exit(1);
                }

                output = fopen(argv[2], "w+");
                if (output == NULL) 
                {
                    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
                    fclose(input);
                    exit(1);
                }

                input_stat = malloc(sizeof(struct stat));
                if (input_stat == NULL)
                {
                    fprintf(stderr, "Malloc failed for input stat.\n");
                    fclose(input);
                    fclose(output);
                    exit(1);
                }

                input_stat_ret = stat(argv[1], input_stat);
                if (input_stat_ret < 0)
                {
                    fprintf(stderr, "Cannot get input file stat\n");
                    free(input_stat);
                    fclose(input);
                    fclose(output);
                    exit(1);
                }

                output_stat = malloc(sizeof(struct stat));
                if (output_stat == NULL)
                {
                    fprintf(stderr, "Malloc failed for output stat.\n");
                    free(input_stat);
                    fclose(input);
                    fclose(output);
                    exit(1);
                }

                output_stat_ret = stat(argv[2], output_stat);

                if (output_stat_ret < 0)
                {
                    fprintf(stderr, "Cannot get output file stat\n");
                    fclose(input);
                    fclose(output);
                    free(input_stat);
                    free(output_stat);
                    exit(1);
                }

                if (input_stat->st_ino == output_stat->st_ino)
                {
                    fprintf(stderr, "reverse: input and output file must differ\n");
                    fclose(input);
                    fclose(output);
                    free(input_stat);
                    free(output_stat);
                    exit(1);
                }

                free(input_stat);
                free(output_stat);
                break;
        default:
                fprintf(stderr, "usage: reverse <input> <output>\n");
                exit(1);
                break;
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
