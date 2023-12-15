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


int add_sll_start(sll** list, const char* str) {
    if (list && *list && str) {
        // list, *list and name cannot be NULL
        sll_node* new_node = (sll_node*)malloc(sizeof(sll_node));
        if (!new_node) {
            // malloc failed to find memory for new_node
            return 0;
        }

        new_node->m_str = (char*)calloc(strlen(str) + 1, sizeof(char));
        if (!new_node->m_str) {
            // calloc failed to find memory for new_node->m_name
            free(new_node);
            return 0;
        }
        strncpy(new_node->m_str, str, strlen(str));  // Copy name into new_node->m_name (the final character will be '\0', guaranteed by calloc)
        new_node->m_strlen = strlen(str);             // Store the number of characters in new_node->m_name (does not include NULL termination character)
        new_node->next = (*list)->head;                 // Set the next pointer to be the current head
        (*list)->head = new_node;                       // Set the head of the list equal to the new node
        (*list)->m_size++;                              // Increment the size of the list
        return 1;
    }
    return 0;   // list, *list and/or name was NULL
}

void delete_sll(sll** list) {
    if (list && *list) {
        // list and *list cannot be NULL
        while ((*list)->head) {
            sll_node* to_delete = (*list)->head;
            (*list)->head = (*list)->head->next;
            free(to_delete->m_str);
            free(to_delete);
        }
        free(*list);
        *list = NULL;
    }
}