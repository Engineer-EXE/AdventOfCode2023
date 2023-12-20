#include "SLL.h"
#include <stdlib.h>

sll* create_sll(void) {
    sll* new_list = (sll*)malloc(sizeof(sll));
    if (new_list) {
        // If malloc did not return NULL, initialise the list head and size
        new_list->head = NULL;
        new_list->m_size = 0;
    }
    return new_list;
}


int add_sll_start(sll** list, int32_t num) {
    if (list && *list) {
        // list and *list cannot be NULL
        sll_node* new_node = (sll_node*)malloc(sizeof(sll_node));
        if (!new_node) {
            // malloc failed to find memory for new_node
            return 0;
        }

        new_node->m_num = num;
        new_node->next = (*list)->head;                 // Set the next pointer to be the current head
        (*list)->head = new_node;                       // Set the head of the list equal to the new node
        (*list)->m_size++;                              // Increment the size of the list
        return 1;
    }
    return 0;   // list and/or *list was NULL
}

sll_node* find_first_elem(const sll* list, int32_t num_to_find) {
    if (list){
        sll_node* current = list->head;

        while(current){
            if (current->m_num == num_to_find){
                return current;
            }
            current = current->next;
        }
    }

    return NULL;
}

void delete_sll(sll** list) {
    if (list && *list) {
        // list and *list cannot be NULL
        while ((*list)->head) {
            sll_node* to_delete = (*list)->head;
            (*list)->head = (*list)->head->next;
            free(to_delete);
        }
        free(*list);
        *list = NULL;
    }
}