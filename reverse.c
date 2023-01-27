// Code by William Joe Brashear
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int debug = 1;

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

void read_text(char *text, size_t len, FILE *input, ListNode *node, ListNode *head)
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
            exit(1);
        }

        node->next->text = (char*)calloc(strlen(text) + 1, sizeof(char));

        if (node->next->text == NULL)
        {
            fprintf(stderr, "Malloc has failed");
            free(text);
            deallocate_List(head);
            exit(1);
        }

        strcpy(node->next->text, text);
        node = node->next;
    }

    return;
}

void reverse(FILE *output, FILE *input) 
{
    char *text = NULL;
    size_t len = 0;
    
    ListNode *node = (ListNode*)calloc(1, sizeof(ListNode));

    if (node == NULL) 
    {
        fprintf(stderr, "Malloc has failed");
        exit(1);
    }

    ListNode *head = node;

    read_text(text, len, input, node, head);

    // free dummy head
    ListNode *temp = head;
    head = head->next;
    free(temp);

    head = reverse_ll(head);
    reverse_to_output(head, output);

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

    

    reverse(output, input);
    
    fclose(input);
    fclose(output);

    return 0;
}
