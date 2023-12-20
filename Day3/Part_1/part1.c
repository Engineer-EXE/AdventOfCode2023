#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include "SLL.h"

#define NUM_SYMBOLS 31
#define TRUE 1
#define FALSE 0

/**
 * @brief Initialises the array a to contain symbols
 * 
 * @param a an array of characters
 * @param size the size of a 
 */
void init_symbols_arr(char a[], int32_t size);

/**
 * @brief Find the indices of input_str that contain symbols stores in the symbols array
 * 
 * @param list a singly-linked list that will store the indices 
 * @param input_str the input string
 * @param input_len the input string's length (does not include NULL termination)
 * @param symbols an array of symbols we want to search for 
 * @param symbols_len the length of the symbols array 
 * @return int8_t a boolean. Returns 1 for success, returns 0 for failure 
 */
int8_t get_symbol_indices(sll** list, const char* input_str, ssize_t input_len, const char symbols[], int32_t symbols_len);

/**
 * @brief Find all the numbers adjacent to the indices of symbols, and return their total
 * 
 * @param symbol_indices singly-linked list of indices of symbols 
 * @param already_checked_list singly-linked list of indices we have already checked
 * @param line the string we wish to search 
 * @param line_len the length of line (does not include NULL termination) 
 * @return int32_t The sum of the numbers found. Returns -1 if it fails 
 */
int32_t find_adjacent_nums(const sll* symbol_indices, sll** already_checked_list, const char* line, ssize_t line_len);

/**
 * @brief Creates sub-string from a longer string. If has been previously been allocated dynamically and is too small to
 * contain the new sub-stirng, this function will automatically resize it and update the number of bytes it has been allocated 
 * (dest_size). If dest is NULL and dest_size is 0, then a new memory will be allocated with calloc. NOTE: dest must be freed using the
 * free() function, even if make_substr fails.
 * 
 * @param dest the string we wish to store the substring in  
 * @param dest_size the size of dest (includes all space alloted to dest, including NULL termination) 
 * @param input the input sting we want to get a sub-string from 
 * @param start the index we wish to start the sub-string from (is included in sub-string) 
 * @param end the index we wish to end the sub-string (is NOT included in sub-string) 
 * @return ssize_t length of the substring (does not necessarily equal the value of dest_size). Return -1 on failure 
 */
ssize_t make_substr(char** dest, size_t* dest_size, const char* input, size_t start, size_t end);

/**
 * @brief Given an index, search both left and right from it to find a continious series of digits (a number)
 * 
 * @param index the start index 
 * @param line the string to serach 
 * @param line_len the length of line (does NOT include NULL termination) 
 * @param already_checked_list singly-linked list of indices we have already checked 
 * @return int32_t the number we found. Returns -1 on failure 
 */
int32_t search_centre(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list);

/**
 * @brief Given an index, search left from it to find a continious series of digits (a number)
 * 
 * @param index the start index 
 * @param line the string to serach 
 * @param line_len the length of line (does NOT include NULL termination) 
 * @param already_checked_list singly-linked list of indices we have already checked 
 * @return int32_t the number we found. Returns -1 on failure 
 */
int32_t search_left(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list);

/**
 * @brief Given an index, search right from it to find a continious series of digits (a number)
 * 
 * @param index the start index 
 * @param line the string to serach 
 * @param line_len the length of line (does NOT include NULL termination) 
 * @param already_checked_list singly-linked list of indices we have already checked 
 * @return int32_t the number we found. Returns -1 on failure 
 */
int32_t search_right(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list);

int main(int argc, char* argv[]){
    
    // Check that a filename is passed in as an argument to main
    if (argc != 2){
        printf("Please pass text file as argument. Quitting!\n");
        exit(EXIT_FAILURE);
    }

    FILE* input_file = fopen(argv[1], "r");

    // Check that the file was opened successfully
    if (!input_file){
        printf("Issue when opening file. Quitting!\n");
        exit(EXIT_FAILURE);
    }

    // Declare and initialise array of symbols
    char symbols[NUM_SYMBOLS];
    init_symbols_arr(symbols, NUM_SYMBOLS);

    // Initialise required variables
    char* line = NULL;
    size_t line_len = 0;
    ssize_t char_read = 0;
    int32_t total = 0;

    // Read in first line
    if ((char_read = getline(&line, &line_len, input_file)) == -1){
        free(line);
        printf("File is empty. Exitting");
        return 0;
    }

    sll *prev_symbols = create_sll(); // Stores the indices of the symbols
    sll* prev_found_nums = create_sll(); // Stores the indices of numbers we have already found
    int8_t success_symbols = get_symbol_indices(&prev_symbols, line, char_read, symbols, NUM_SYMBOLS); // Find and store the indices where the symbols are
    
    // Ensure get_symbol_indices didn't fail
    if (!success_symbols) {
        free(line);
        delete_sll(&prev_symbols);
        delete_sll(&prev_found_nums);
        printf("Issue with get_symbol_indices. Exitting\n");
        exit(EXIT_FAILURE);
    }

    // Find the adjacent numbers
    int32_t subtotal = find_adjacent_nums(prev_symbols, &prev_found_nums, line, char_read);

    if (subtotal == -1) {
        free(line);
        delete_sll(&prev_symbols);
        delete_sll(&prev_found_nums);
        printf("Issue with find_adjacent_nums. Exitting\n");
        exit(EXIT_FAILURE);
    }
    total += subtotal;     

    // Store the current string and its length
    char* prev_line = (char*)calloc(line_len, sizeof(char));
    if (!prev_line){
        free(line);
        delete_sll(&prev_symbols);
        delete_sll(&prev_found_nums);
        printf("Issue with find_adjacent_nums. Exitting\n");
        exit(EXIT_FAILURE);
    }
    strncpy(prev_line, line, strlen(line) + 1);
    ssize_t prev_char_read = char_read;

    sll *current_symbols = create_sll();
    sll *current_found_nums = create_sll();

    // Read in the subsequent lines
    while ((char_read = getline(&line, &line_len, input_file)) != -1) {

        int8_t success_symbols = get_symbol_indices(&current_symbols, line, char_read, symbols, NUM_SYMBOLS);

        if (!success_symbols) {
            free(line);
            free(prev_line);
            delete_sll(&prev_symbols);
            delete_sll(&current_symbols);
            delete_sll(&prev_found_nums);
            delete_sll(&current_found_nums);
            printf("Issue with get_symbol_indices. Exitting\n");
            exit(EXIT_FAILURE);
        }

        // Find the adjacent numbers for current line and current line's symbols
        int32_t subtotal = find_adjacent_nums(current_symbols, &current_found_nums, line, char_read);

        if (subtotal == -1) {
            free(line);
            free(prev_line);
            delete_sll(&prev_symbols);
            delete_sll(&prev_found_nums);
            delete_sll(&current_symbols);
            delete_sll(&current_found_nums);
            printf("Issue with find_adjacent_nums. Exitting\n");
            exit(EXIT_FAILURE);
        }
        total += subtotal; 

        // Find the adjacent numbers for current line and previous line's symbols
        subtotal = find_adjacent_nums(prev_symbols, &current_found_nums, line, char_read);

        if (subtotal == -1) {
            free(line);
            free(prev_line);
            delete_sll(&prev_symbols);
            delete_sll(&prev_found_nums);
            delete_sll(&current_symbols);
            delete_sll(&current_found_nums);
            printf("Issue with find_adjacent_nums. Exitting\n");
            exit(EXIT_FAILURE);
        }
        total += subtotal; 

        // Find the adjacent numbers for previoud line and current line's symbols
        subtotal = find_adjacent_nums(current_symbols, &prev_found_nums, prev_line, prev_char_read);

        if (subtotal == -1) {
            free(line);
            free(prev_line);
            delete_sll(&prev_symbols);
            delete_sll(&prev_found_nums);
            delete_sll(&current_symbols);
            delete_sll(&current_found_nums);
            printf("Issue with find_adjacent_nums. Exitting\n");
            exit(EXIT_FAILURE);
        }
        total += subtotal; 

    // Store the current string and its length
        free(prev_line);
        prev_line = (char*)calloc(line_len, sizeof(char));
        if (!prev_line){
            free(line);
            delete_sll(&prev_symbols);
            delete_sll(&prev_found_nums);
            delete_sll(&current_symbols);
            delete_sll(&current_found_nums);
            printf("Issue with find_adjacent_nums. Exitting\n");
            exit(EXIT_FAILURE);
        }
        strncpy(prev_line, line, strlen(line) + 1);
        prev_char_read = char_read;

        delete_sll(&prev_symbols);
        delete_sll(&prev_found_nums);
        prev_symbols = current_symbols;
        prev_found_nums = current_found_nums;
        current_symbols = create_sll();
        current_found_nums = create_sll();
    }

    printf("Total = %d\n", total);

    free(line);
    free(prev_line);
    delete_sll(&prev_symbols);
    delete_sll(&prev_found_nums);
    fclose(input_file);
    return 0;
}


void init_symbols_arr(char a[], int32_t size) {
    int32_t i = 0; // To keep track of index

    for (char c = 0x21; c <= 0x2D; c++){
        if (i < size) 
            a[i] = c;
        i++;
    }

    if (i < size)
        a[i] = 0x2F;

    i++;

    for (char c = 0x3A; c <= 0x40; c++){
        if (i < size) 
            a[i] = c;
        i++;
    }

    for (char c = 0x5B; c <= 0x60; c++){
        if (i < size)
            a[i] = c;
        i++;
    }

    for (char c = 0x7B; c <= 0x7E; c++){
        if (i < size)
            a[i] = c;
        i++;
    }
}

int8_t get_symbol_indices(sll** list, const char* input_str, ssize_t input_len, const char symbols[], int32_t symbols_len) {
    // Check that list and *list are not NULL
    if (list && *list) {
        // Iterate over the each of the characters of input_str
        for (ssize_t i = 0; i < input_len; i++) {
            // Iterate over each symbol and check the current character of input_str is a match
            for (int32_t j = 0; j < symbols_len; j++) {
                if (input_str[i] == symbols[j]) {
                    add_sll_start(list, i);
                    break;
                }
            }
        }
        return 1;
    }

    return 0; // list or *list were NULL
}

int32_t find_adjacent_nums(const sll* symbol_indices, sll** already_checked_list, const char* line, ssize_t line_len) {
    // Check that symbol_indices, already_checked_list, *already_checked_list and line are not NULL
    if (symbol_indices && already_checked_list && *already_checked_list && line){
        sll_node* current = symbol_indices->head; // The current node in the SLL we are looking at
        int32_t total = 0; // Stores the sum of the numbers we find

        while (current){
            // If we haven't checked this index before and it contains a digit
            if (!find_first_elem(*already_checked_list, current->m_num) && line[current->m_num] >= '0' && line[current->m_num] <= '9'){
                int32_t num = search_centre(current->m_num, line, line_len, already_checked_list);
                
                // search_centre failed
                if (num == -1)
                    return -1;

                total += num; // search_centre succeeded. Add number to total
            } else {
                // If we haven't checked the index to the left of the current index
                // If this index is not less than 0
                // If the index contains a digit
                if (!find_first_elem(*already_checked_list, current->m_num - 1) && (current->m_num - 1) >= 0 && line[current->m_num - 1] >= '0' && line[current->m_num - 1] <= '9'){
                    int32_t num = search_left(current->m_num - 1, line, line_len, already_checked_list);
                    
                    // search_left failed
                    if (num == -1)
                        return -1;

                    total += num; // search_left succeeded. Add number to total
                }

                // If we haven't checked the index to the right of the current index
                // If this index is less than the length of the line 
                // If the index contains a digit
                if (!find_first_elem(*already_checked_list, current->m_num + 1) && (current->m_num + 1) < line_len && line[current->m_num + 1] >= '0' && line[current->m_num + 1] <= '9'){
                    int32_t num = search_right(current->m_num + 1, line, line_len, already_checked_list);

                    // search_right failed
                    if (num == -1)
                        return -1;

                    total += num; // search_right succeeded. Add number to total
                }
            }
            current = current->next; // Go to next node
        }

        return total; // Return the total sum
    }
    return -1; // symbol_indices, already_checked_list, *already_checked_list or line was NULL
}

ssize_t make_substr(char** dest, size_t* dest_size, const char* input, size_t start, size_t end){
    // Check that the input string is not NULL
    if (!input){
        printf("input is NULL. Exiting function make_substr\n");
        return -1;
    }

    // Check that start and end values make sense
    if (end <= start || start > strlen(input) || end > strlen(input)) {
        printf("Invalid values of start and/or end. Exiting function make_substr\n");
        return -1;
    }

    size_t min_len = (end - start + 1); // Minimum length needed by dest to fit the string (includes NULL termination)

    // If the dest pointer is NULL and its size is 0, we allocate heap memory
    if (*dest == NULL && *dest_size == 0){
        *dest = (char*)calloc(min_len, sizeof(char));
        // Check if calloc was successfull
        if (!(*dest)){
            printf("Failed to calloc array. Exiting function make_substr\n");
            return -1;
        }
        // Update the value of dest_size to be the amount of bytes allocated
        *dest_size = min_len;
    }

    // If there isn't enough space in dest we need to increase its size
    if (*dest_size < min_len){
        char* new_dest = (char*)reallocarray(*dest, min_len, sizeof(char));

        // Check if reallocarray was successfull
        if(!new_dest){
            printf("Failed to realloc array. Exiting function make_substr\n");
            return -1;
        }

        // Update dest and dest_size
        *dest = new_dest;
        *dest_size = min_len;
    }

    // Copy the contents of input into dest. Include the character with index starr
    // Exclude the character with index end
    size_t j = 0;
    for (size_t i = start; i < end; i++){
        (*dest)[j] = input[i];
        j++;
    }

    // Null-terminate the string
    (*dest)[j] = '\0';

    // Return the length of the string
    return strlen(*dest);
}

int32_t search_centre(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list) {
    // Check that already_checked_list and *already_checked_list are not NULL
    if (already_checked_list && *already_checked_list) {

        int32_t start = index;
        int32_t end = index + 1;

        int32_t i; // Index
        int8_t halted_left = FALSE;
        int8_t halted_right = FALSE;

        // Start going left from centre
        for (i = index; i >= 0; i--) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The start should store the last character that is a number
                start = i + 1;
                halted_left = TRUE;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where index 0 is also a number
        if (!halted_left && line[0] >= '0' && line[0] <= '9')
            start = 0;

        // Start going right from centre
        for (i = index; i < line_len; i++) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The end should store the character to the right of the last number
                end = i;
                halted_right = TRUE;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where the final character is a number
        if (!halted_right && line[i - 1] >= '0' && line[i - 1] <= '9')
            end = i; // In this case the character at this index is '\0'
    
        char* new_str = NULL;
        size_t new_str_len = 0;

        ssize_t success = make_substr(&new_str, &new_str_len, line, start, end);
        if (success == -1){
            free(new_str);
            return -1;
        }

        int32_t num = atoi(new_str);
        free(new_str);

        return num;
    }

    return -1; // already_checked_list or *already_checked_list was NULL
}

int32_t search_left(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list) {
    if (already_checked_list && *already_checked_list) {

        int32_t start = index;
        int32_t end = index + 1;

        int32_t i; // Index
        int8_t halted = FALSE;

        // Start going left from centre
        for (i = index; i >= 0; i--) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The start should store the last character that is a number
                start = i + 1;
                halted = TRUE;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where index 0 is also a number
        if (!halted && line[0] >= '0' && line[0] <= '9')
            start = 0;
    
        char* new_str = NULL;
        size_t new_str_len = 0;

        ssize_t success = make_substr(&new_str, &new_str_len, line, start, end);
        if (success == -1){
            free(new_str);
            return -1;
        }

        int32_t num = atoi(new_str);
        free(new_str);

        return num;
    }

    return -1;
}

int32_t search_right(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list) {
    if (already_checked_list && *already_checked_list) {

        int32_t start = index;
        int32_t end = index;

        int32_t i; // Index
        int8_t halted = FALSE;

        // Start going right from centre
        for (i = index; i < line_len; i++) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The end should store the character to the right of the last number
                end = i;
                halted = TRUE;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where the final character is a number
        if (!halted && line[i - 1] >= '0' && line[i - 1] <= '9')
            end = i; // In this case the character at this index is '\0'
    
        char* new_str = NULL;
        size_t new_str_len = 0;

        ssize_t success = make_substr(&new_str, &new_str_len, line, start, end);
        if (success == -1){
            free(new_str);
            return -1;
        }

        int32_t num = atoi(new_str);
        free(new_str);

        return num;
    }

    return -1;
}