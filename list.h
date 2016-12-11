#ifndef LIST_H
#define LIST_H
#include <stdio.h>


struct node {
    void *data;
    struct node *next;
};

void push_element(struct node **head, void *new_data, size_t data_size);
void erase_element(struct node **head, int n);

#endif
