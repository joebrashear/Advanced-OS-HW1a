// Code by William Joe Brashear
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    fprintf(stderr, "Input and output file must differ");
    return 1;
}

void reverse_to_output(ListNode *head, FILE *output)
{
    ListNode *temp = NULL;
    int node_count = 1;
    // add to output and deallocate LL
    while (head != NULL)
    {
        fprintf(output, "%s", head->text);
        if (node_count == 1)
        {
            fprintf(output, "\n");
            node_count++;
        }
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
            fprintf(stderr, "Malloc has failed");
            free(text);
            deallocate_List(head);
            return 1;
        }

        node->next->text = (char*)calloc(len, sizeof(char));

        if (node->next->text == NULL)
        {
            fprintf(stderr, "Malloc has failed");
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
        fprintf(stderr, "Malloc has failed");
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
        fprintf(stderr, "reverse: cannot open file '%s'", argv[1]);
        exit(1);
    }

    output = fopen(argv[2], "r+");

    if (output == NULL) 
    {
        fprintf(stderr, "reverse: cannot open file '%s'", argv[2]);
        fclose(input);
        exit(1);
    }

    

    if (reverse(output, input) == 1) 
    {
        fclose(input);
        fclose(output);
        exit(1);
    }

    if (is_same(input, output) == 1) 
    {
        fclose(input);
        fclose(output);
        exit(1);
    }
    
    fclose(input);
    fclose(output);

    return 0;
}
