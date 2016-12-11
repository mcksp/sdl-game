#include "list.h"
#include <string.h>
#include <stdlib.h>

void push_element(struct node **head, void *data, size_t data_size) {
    struct node* new_node = malloc(sizeof(struct node));
    new_node->data = malloc(data_size);
    new_node->next = *head;
    memcpy(new_node->data, data, data_size);
    *head = new_node;
}

void erase_element(struct node **head, int n) {
    int i;
    struct node *tmp = *head;
    if (n == 0) {
        tmp = (*head)->next;
        free((*head)->data);
        free(*head);
        *head = tmp;
    } else {
        for (i = 0; i < n - 1; i++) {
            if (tmp->next != NULL) {
                tmp = tmp->next;
            } else {
                break;
            }
        }
        struct node *next = NULL; 
        if (tmp->next != NULL) {
            next = tmp->next->next;
        }
        free(tmp->next->data);
        free(tmp->next);
        tmp->next = next;
    }
}
        
