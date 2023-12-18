#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include "SLL.h"

#define NUM_SYMBOLS 31
#define TRUE 1
#define FALSE 0

void init_symbols_arr(char a[], int32_t size);
int8_t get_symbol_indices(sll** list, const char* input_str, ssize_t input_len, const char symbols[], int32_t symbols_len);
int32_t find_adjacent_nums(const sll* symbol_indices, sll** already_checked_list, const char* line, ssize_t line_len);
ssize_t make_substr(char** dest, size_t* dest_size, const char* input, size_t start, size_t end);
int32_t search_centre(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list);
int32_t search_left(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list);
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

    if ((char_read = getline(&line, &line_len, input_file)) == -1){
        free(line);
        printf("File is empty. Exitting");
        return 0;
    }

    printf("Read in: %s\n", line);

    sll *prev_symbols = create_sll();
    sll* prev_found_nums = create_sll();
    int8_t success_symbols = get_symbol_indices(&prev_symbols, line, char_read, symbols, NUM_SYMBOLS);
    

    if (!success_symbols) {
        free(line);
        delete_sll(&prev_symbols);
        printf("Issue with get_symbol_indices. Exitting\n");
        exit(EXIT_FAILURE);
    }

    printf("Symbols found at: ");

    sll_node *current = prev_symbols->head;
    while (current) {
        printf("%d, ", current->m_num);
        current = current->next;
    }
    printf("\n");

    int32_t subtotal = find_adjacent_nums(prev_symbols, &prev_found_nums, line, char_read);
    printf("Total of adjacent nums = %d\n", subtotal);
    printf("\n---------------------------------------------------\n");
    delete_sll(&prev_symbols);
    delete_sll(&prev_found_nums);

    while ((char_read = getline(&line, &line_len, input_file)) != -1) {
        printf("Read in: %s\n", line);
        sll *prev_symbols = create_sll();
        sll *prev_found_nums = create_sll();
        int8_t success_symbols = get_symbol_indices(&prev_symbols, line, char_read, symbols, NUM_SYMBOLS);

        if (!success_symbols) {
            free(line);
            delete_sll(&prev_symbols);
            printf("Issue with get_symbol_indices. Exitting\n");
            exit(EXIT_FAILURE);
        }

        printf("Symbols found at: ");

        sll_node *current = prev_symbols->head;
        while (current) {
            printf("%d, ", current->m_num);
            current = current->next;
        }
        printf("\n");

        int32_t subtotal = find_adjacent_nums(prev_symbols, &prev_found_nums, line, char_read);
        printf("Total of adjacent nums = %d\n", subtotal);
        printf("\n---------------------------------------------------\n");
        delete_sll(&prev_symbols);
        delete_sll(&prev_found_nums);
    }

    free(line);
    fclose(input_file);
    return 0;
}


void init_symbols_arr(char a[], int32_t size) {
    // Declare and initialise array of symbols 
    int32_t i = 0;
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
    if (list != NULL && *list != NULL) {
        for (ssize_t i = 0; i < input_len; i++) {
            for (int32_t j = 0; j < symbols_len; j++) {
                if (input_str[i] == symbols[j]) {
                    add_sll_start(list, i);
                    break;
                }
            }
        }
        return 1;
    }

    return 0;
}


int32_t find_adjacent_nums(const sll* symbol_indices, sll** already_checked_list, const char* line, ssize_t line_len) {
    if (symbol_indices != NULL && already_checked_list != NULL && *already_checked_list != NULL && line != NULL){
        sll_node* current = symbol_indices->head;
        int32_t total = 0;

        while (current){
            if (!find_first_elem(*already_checked_list, current->m_num) && line[current->m_num] >= '0' && line[current->m_num] <= '9'){
                int32_t num = search_centre(current->m_num, line, line_len, already_checked_list);
                if (num == -1)
                    return -1;

                total += num;
            } else {
                if (!find_first_elem(*already_checked_list, current->m_num - 1) && (current->m_num - 1) >= 0 && line[current->m_num - 1] >= '0' && line[current->m_num - 1] <= '9'){
                    int32_t num = search_left(current->m_num - 1, line, line_len, already_checked_list);
                    if (num == -1)
                        return -1;

                    total += num;
                }

                if (!find_first_elem(*already_checked_list, current->m_num + 1) && (current->m_num + 1) < line_len && line[current->m_num + 1] >= '0' && line[current->m_num + 1] <= '9'){
                    int32_t num = search_right(current->m_num + 1, line, line_len, already_checked_list);
                    if (num == -1)
                        return -1;

                    total += num;
                }
            }
            current = current->next;
        }
        return total;
    }
    return -1;
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
        *dest = (char*)calloc(sizeof(char), min_len);
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
        char* new_dest = (char*)reallocarray(*dest, sizeof(char), min_len);

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
    if (already_checked_list && *already_checked_list) {

        int32_t start = index;
        int32_t end = index + 1;

        int32_t i; // Index

        // Start going left from centre
        for (i = index; i >= 0; i--) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The start should store the last character that is a number
                start = i + 1;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where index 0 is also a number
        if (line[i] >= '0' && line[i] <= '9')
            start = i;

        // Start going right from centre
        for (i = index; i < line_len; i++) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The end should store the character to the right of the last number
                end = i;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where the final character is a number
        if (line[i] >= '0' && line[i] <= '9')
            end = i + 1; // In this case the character at this index is '\0'
    
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

int32_t search_left(int32_t index, const char* line, ssize_t line_len, sll** already_checked_list) {
    if (already_checked_list && *already_checked_list) {

        int32_t start = index;
        int32_t end = index + 1;

        int32_t i; // Index

        // Start going left from centre
        for (i = index; i >= 0; i--) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The start should store the last character that is a number
                start = i + 1;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where index 0 is also a number
        if (line[i] >= '0' && line[i] <= '9')
            start = i;
    
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

        // Start going right from centre
        for (i = index; i < line_len; i++) {
            // Check if the current character is a number or not
            if (line[i] < '0' || line[i] > '9') {
                // Current character is no longer a number
                // The end should store the character to the right of the last number
                end = i;
                break;
            }
            // Current character is still a number. Add it to the list of indices of numbers
            add_sll_start(already_checked_list, i);
        }

        // Additional check to deal with case where the final character is a number
        if (line[i] >= '0' && line[i] <= '9')
            end = i + 1; // In this case the character at this index is '\0'
    
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