#include <stdlib.h>
#include "slist.h"

SNode* add_node(SNode *head, int value) {
    SNode *new_node = malloc(sizeof(SNode));
    if (!new_node) return head;  // Fail gracefully
    new_node->value = value;
    new_node->next = head;
    return new_node;
}

int count_nodes(SNode *head) {
    int count = 0;
    for (SNode *p = head; p; p = p->next) {
        count++;
    }
    return count;
}

void free_list(SNode *head) {
    while (head) {
        SNode *next = head->next;
        free(head);
        head = next;
    }
}
