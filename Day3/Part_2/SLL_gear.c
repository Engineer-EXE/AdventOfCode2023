#include "SLL_gear.h"
#include <stdlib.h>

sll_gear* create_sll_gear(void) {
    sll_gear* new_list = (sll_gear*)malloc(sizeof(sll_gear));
    if (new_list) {
        // If malloc did not return NULL, initialise the list head and size
        new_list->head = NULL;
        new_list->m_size = 0;
    }
    return new_list;
}


int add_sll_gear_start(sll_gear** list, int32_t index) {
    if (list && *list) {
        // list and *list cannot be NULL
        sll_node_gear* new_node = (sll_node_gear*)malloc(sizeof(sll_node_gear));
        if (!new_node) {
            // malloc failed to find memory for new_node
            return 0;
        }

        new_node->m_index = index;
        new_node->m_num_adjacent = 0;
        new_node->adjacent_nums = create_sll();

        if (!new_node->adjacent_nums) {
            free(new_node);
            return 0;
        }

        new_node->next = (*list)->head;                 // Set the next pointer to be the current head
        (*list)->head = new_node;                       // Set the head of the list equal to the new node
        (*list)->m_size++;                              // Increment the size of the list
        return 1;
    }
    return 0;   // list and/or *list was NULL
}

void delete_sll_gear(sll_gear** list) {
    if (list && *list) {
        // list and *list cannot be NULL
        while ((*list)->head) {
            sll_node_gear* to_delete = (*list)->head;
            (*list)->head = (*list)->head->next;
            delete_sll(to_delete->adjacent_nums);
            free(to_delete);
        }
        free(*list);
        *list = NULL;
    }
}