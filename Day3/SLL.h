#ifndef _JP_STDLIB_SLL
#define _JP_STDLIB_SLL

#include <string.h>
#include <stdint.h>

typedef struct sll_node sll_node;

struct sll_node {
    int32_t m_num;      // Stores an integer  
    sll_node* next;     // Pointer to the next node in the list
};

typedef struct {
    sll_node* head;      // Pointer to the head of the list
    unsigned int m_size; // Size of the list (i.e. number of nodes)
} sll;


/**
 * @brief Creates an empty linked list. The list is initialised so that
 *        the head points to NULL, and the list's size is 0
 * 
 * @return sll* Pointer to empty linked list on success. Returns NULL on failure
 */
sll* create_sll(void);


/**
 * @brief Adds a new item at the start of the linked list
 * 
 * @param list Linked list to which we want to add
 * @param name Integer we wish to add to the list
 * @return int 1 upon success, 0 upon failure
 */
int add_sll_start(sll** list, int32_t num);

/**
 * @brief Deletes the passed linked list, freeing all dynamically-allocated
 *        memory. It also sets the single pointer to the list equal to NULL.
 * 
 * @param list The linked list we want to delete
 */
void delete_sll(sll** list);

#endif //_JP_STDLIB_SLL