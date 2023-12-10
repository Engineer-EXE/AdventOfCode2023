#include<stdio.h>
#include<string.h>
#include<stdlib.h>

const int MAX_CUBES[3] = {12, 13, 14};
const char* COLOURS[3] = {"red", "green", "blue"};

int make_substr(char** dest, size_t* dest_size, const char* input, int start, int end);
void preprocess_input(char** input_str);
void find_max_colours(const char* input_str, int a[], int size_a);



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
    __ssize_t chars_read = 0;

    
    while((chars_read = getline(&line, &n, input_file)) != -1){
        printf("Read new line:\n%s\n", line);
        preprocess_input(&line);
        // printf("After preprocessing, line is:\n%s\n\n", line);

        int a[1] = {0};
        int len = 1;
        find_max_colours(line, a, len);
    }
    
    free(line);
    fclose(input_file);

    return 0;
}


int make_substr(char** dest, size_t* dest_size, const char* input, int start, int end){
    if (input == NULL)
        return -1;

    if (start < 0 || end <= start || start > strlen(input) || end > strlen(input))
        return -1;

    if (*dest == NULL && *dest_size == 0){
        *dest = (char*)calloc(sizeof(char), strlen(input) + 1);
        if (*dest == NULL)
            return -1;

        *dest_size = strlen(input) + 1;
    }

    if (*dest_size < (strlen(input) + 1)){
        char* new_dest = (char*)reallocarray(*dest, sizeof(char), strlen(input) + 1);

        if(new_dest == NULL){
            printf("Failed to realloc array. dest has been freed\n");
            return -1;
        }

        *dest = new_dest;
        *dest_size = strlen(input) + 1;
    }
    int j = 0;
    for (int i = start; i < end; i++){
        (*dest)[j] = input[i];
        j++;
    }
    (*dest)[end] = '\0';
    return strlen(*dest);
}



void preprocess_input(char** input_str){
    if (input_str != NULL && (*input_str) != NULL){
        int len = strlen(*input_str);
        int encountered_delim = 0;
        char delim = ':';
        for (int i = 0; i < len; i++){
            
            if ((*input_str)[i] == delim){
                // we encountered the delimiter. Set flag to true
                encountered_delim = 1;
            } else if (encountered_delim && (*input_str)[i] != ' '){
                // we encountered the delimited in the past and have also read
                // past all the whitespace after the delimiter
                char* new_str = NULL;
                size_t len_new_str = 0;
                int success = make_substr(&new_str, &len_new_str, *input_str, i, strlen(*input_str) - 1);
                if (success == -1){
                    free(new_str);
                    return;
                }
                strncpy(*input_str, new_str, strlen(new_str) + 1); // Make the input string equal to the substring
                free(new_str);
                break;
            }
        }
    }
}


void find_max_colours(const char* input_str, int a[], int size_a){
    if (input_str == NULL)
        return;
        
    char* one_game = NULL;
    size_t len_one_game = 0;
    int curr_start = 0;
    int game_created = 0;

    for (int i = 0; i < strlen(input_str); i++){
        if (game_created){
            curr_start = i;
            game_created = 0;
        }
        if (input_str[i] == ';' || input_str[i] == '\n' || input_str[i] == '\0'){
            int success = make_substr(&one_game, &len_one_game, input_str, curr_start, i);
            if (success == -1){
                free(one_game);
                return;
            }
            game_created = 1;
            printf("One game: %s\n", one_game);
        }
    }
    free(one_game);
}
