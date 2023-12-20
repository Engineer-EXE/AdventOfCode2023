#ifndef _JP_STDLIB_SLL_GEAR
#define _JP_STDLIB_SLL_GEAR

#include <string.h>
#include <stdint.h>
#include "SLL.h"
typedef struct sll_node_gear sll_node_gear;

struct sll_node_gear {
    int32_t m_index;            // Stores index  
    int32_t m_num_adjacent;     // Stores number of adjacent numbers
    sll* adjacent_nums;
    sll_node_gear* next;        // Pointer to the next node in the list
};

typedef struct {
    sll_node_gear* head;      // Pointer to the head of the list
    unsigned int m_size; // Size of the list (i.e. number of nodes)
} sll_gear;


/**
 * @brief Creates an empty linked list. The list is initialised so that
 *        the head points to NULL, and the list's size is 0
 * 
 * @return sll* Pointer to empty linked list on success. Returns NULL on failure
 */
sll_gear* create_sll_gear(void);


/**
 * @brief Adds a new item at the start of the linked list
 * 
 * @param list Linked list to which we want to add
 * @param name Integer we wish to add to the list
 * @return int 1 upon success, 0 upon failure
 */
int add_sll_gear_start(sll_gear** list, int32_t index);


/**
 * @brief Deletes the passed linked list, freeing all dynamically-allocated
 *        memory. It also sets the single pointer to the list equal to NULL.
 * 
 * @param list The linked list we want to delete
 */
void delete_sll_gear(sll_gear** list);

#endif //_JP_STDLIB_SLL_GEAR