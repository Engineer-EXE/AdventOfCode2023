#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include "SLL.h"

#define TRUE 1
#define FALSE 0

const char* COLOURS[3] = {"red", "green", "blue"};

ssize_t make_substr(char** dest, size_t* dest_size, const char* input, size_t start, size_t end);
ssize_t preprocess_input(char** input_str);
uint8_t separate_colours(const char* input_str, sll** list);
uint8_t find_max_colours(const char* input_str, uint32_t a[], uint32_t a_size);
int32_t find_number_in_str(const char* input);

int main(int argc, char* argv[]){

    if (argc != 2){
        printf("Please pass text file as argument. Quitting!\n");
        exit(EXIT_FAILURE);
    }

    FILE* input_file = fopen(argv[1], "r");

    if (input_file == NULL){
        printf("Issue when opening file. Quitting!\n");
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t n = 0;
    ssize_t chars_read = 0;
    int32_t total = 0; 

    while((chars_read = getline(&line, &n, input_file)) != -1){
        // printf("Read new line:\n%s\n", line);
        size_t preprocess_success = preprocess_input(&line);
        if (preprocess_success == -1){
            printf("Issue with preprocess_input\n");
            continue;
        }
        // printf("After preprocessing, line is:\n%s\n", line);
        
        uint32_t size_array = 3;
        uint32_t colours[3] = {0, 0, 0};
        uint8_t success_find_max = find_max_colours(line, colours, size_array);
        if (!success_find_max){
            printf("Issue find_max_number\n");
            continue;
        }
        total += colours[0] * colours[1] * colours[2];
    }

    printf("Total = %d\n", total);
    
    free(line);
    fclose(input_file);

    return 0;
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

ssize_t preprocess_input(char** input_str) {
    if (!input_str && !(*input_str)){
        printf("NULL passed as input. Exitting preporccess_input function\n");
        return -1;
    }

    size_t len = strlen(*input_str);
    int8_t encountered_delim = FALSE;
    char delim = ':';

    for (size_t i = 0; i < len; i++){
        if ((*input_str)[i] == delim){
            // we encountered the delimiter. Set flag to true
            encountered_delim = TRUE;
        } else if (encountered_delim && (*input_str)[i] != ' '){
            // we encountered the delimited in the past and have also read
            // past all the whitespace after the delimiter
            char* new_str = NULL;
            size_t len_new_str = 0;
            ssize_t success = make_substr(&new_str, &len_new_str, *input_str, i, strlen(*input_str));
            if (success == -1){
                printf("Issue with the make_substr function call. Exitting preprocess_input function\n");
                free(new_str);
                return -1;
            }
            strncpy(*input_str, new_str, strlen(new_str) + 1); // Make the input string equal to the substring
            free(new_str);
            return strlen(*input_str); 
        }
    }
    return strlen(*input_str);
}

uint8_t separate_colours(const char* input_str, sll** list){
    if (!input_str && !list && !(*list)){
        printf("Value passed is NULL. Exitting separate_colours function\n");
        return 0;
    }

    size_t input_len = strlen(input_str);
    int8_t on_delim_char = FALSE;
    size_t curr_start = 0;

    for(size_t i = 0; i < input_len; i++){
        if (!on_delim_char && (input_str[i] == ',' || input_str[i] == ';')){
            on_delim_char = TRUE;
            char* new_str = NULL;
            size_t new_str_len = 0;

            ssize_t success = make_substr(&new_str, &new_str_len, input_str, curr_start, i);
            if (success == -1){
                printf("Issue with make_substr. Exitting separate_colours function\n");
                free(new_str);
                return 0;
            }
            add_sll_start(list, new_str);
            free(new_str);
        }

        if (!on_delim_char && input_str[i] == '\n'){
            on_delim_char = TRUE;
            char* new_str = NULL;
            size_t new_str_len = 0;

            ssize_t success = make_substr(&new_str, &new_str_len, input_str, curr_start, i);
            if (success == -1){
                printf("Issue with make_substr. Exitting separate_colours function\n");
                free(new_str);
                return 0;
            }
            add_sll_start(list, new_str);
            free(new_str);
            return 1;
        }

        if (on_delim_char && (input_str[i] != ',' && input_str[i] != ';')){
            curr_start = i;
            on_delim_char = FALSE;
        }
    }

   if (!on_delim_char){
            on_delim_char = TRUE;
            char* new_str = NULL;
            size_t new_str_len = 0;

            ssize_t success = make_substr(&new_str, &new_str_len, input_str, curr_start, strlen(input_str));
            if (success == -1){
                printf("Issue with make_substr. Exitting separate_colours function\n");
                free(new_str);
                return 0;
            }
            add_sll_start(list, new_str);
            free(new_str);
            return 1;
   } 

    return 1;

}

uint8_t find_max_colours(const char* input_str, uint32_t a[], uint32_t a_size) {
    if (!input_str){
        printf("NULL string passed. Exitting find_max_colours\n");
        return 0;
    }

    if (a_size != 3){
        printf("The array must be size 2. Exitting find_max_colour\n");
        return 0;
    }

    sll* list = create_sll();
    uint8_t separated_success = separate_colours(input_str, &list);
    if (!separated_success){
        printf("Issue with separating colours. Exitting find_max_colour\n");
        delete_sll(&list);
        return 0;
    }

    sll_node* current = list->head;
    while(current){
        int32_t number = find_number_in_str(current->m_str);
        if (number == -1){
            printf("Issue with finding number in string. Exitting find_max_colour\n");
            delete_sll(&list);
            return 0;
        }

        if (strstr(current->m_str, COLOURS[0])){
            if (number > a[0])
                a[0] = number;
        } else if (strstr(current->m_str, COLOURS[1])){
            if (number > a[1])
                a[1] = number;
        } else if (strstr(current->m_str, COLOURS[2])){
            if (number > a[2])
                a[2] = number;
        } else {
            printf("The string does not contain a colour. Exitting find_max_colour\n");
            delete_sll(&list);
            return 0;
        }

        current = current->next;
    }

    delete_sll(&list);
    return 1;
}

int32_t find_number_in_str(const char* input) {
    // Check that the input string is not NULL
    if (!input){
        printf("input is NULL. Exiting function make_substr\n");
        return -1;
    }

    int8_t found_num = FALSE;
    size_t start_index = 0;

    for (size_t i = 0; i <= strlen(input); i++){
        if (input[i] >= '0' && input[i] <= '9'){
            if (!found_num){
                found_num = TRUE;
                start_index = i;
            }
        } else if (found_num){
            char* new_str = NULL;
            size_t new_str_len = 0;

            ssize_t success = make_substr(&new_str, &new_str_len, input, start_index, i);      
            if (success == -1){
                printf("Issue with make_substr in find_number_in_str. Exitting make_substr\n");
                free(new_str);
                return -1;
            }

            int32_t number = atoi(new_str);
            free(new_str);
            return number;
        } 
    }

    return -1;
    
}