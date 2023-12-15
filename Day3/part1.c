#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

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
    char symbols[32];
    int32_t i = 0;
    for (char c = 0x21; c <= 0x2F; c++){
        if (i < 32) 
            symbols[i] = c;
        i++;
    }

    for (char c = 0x3A; c <= 0x40; c++){
        if (i < 32) 
            symbols[i] = c;
        i++;
    }

    for (char c = 0x5B; c <= 0x60; c++){
        if (i < 32)
            symbols[i] = c;
        i++;
    }

    for (char c = 0x7B; c <= 0x7E; c++){
        if (i < 32)
            symbols[i] = c;
        i++;
    }

    // Initialise required variables
    char* line = NULL;
    size_t line_len = 0;
    ssize_t char_read = 0;

    while((char_read = getline(&line, &line_len, input_file)) != -1){
        printf("Read in: %s\n", line);
    }

    free(line);
    fclose(input_file);
    return 0;
}