#ifndef LIST_H
#define LIST_H
#include <stdio.h>


struct node {
    void *data;
    struct node *next;
};

void push(struct node **head, void *new_data, size_t data_size);
void erase(struct node **head, int n);

#endif
