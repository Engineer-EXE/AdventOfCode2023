#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char** argv){

	// Check if input file was passed as an argument
	if (argc != 2){
		printf("Please pass a filename as an argument! Quitting!\n");
		exit(EXIT_FAILURE);
	}

	// Attempt to open file
	FILE* text_in = fopen(argv[1], "r");

	// Ensure file has been opened successfully
	if (text_in == NULL){
		perror("Issue encountered while opening file! Quitting!\n");
		exit(EXIT_FAILURE);
	}

	// Initialise variables
	char* line = NULL; // Stores the current line read from the text file
	size_t line_len = 0; // Stores the length of line
	int total = 0; // Stores the running total of numbers found
	ssize_t char_read = 0; // Number of characters read by getline()

	// Read in and check each line of the text file
	while((char_read = getline(&line, &line_len, text_in)) != -1){

		int num1 = 0; // Stores the leftmost number
		int num2 = 0; // Stores the rightmost number

		// Find the leftmost digit
		for (int i = 0; i < char_read; i++){
			if (line[i] >= '0' && line[i] <= '9'){
				num1 = line[i] - '0';
				break;
			}
		}

		// Find the rightmost digit
		for (int i = char_read - 1; i >= 0; i--){
			if (line[i] >= '0' && line[i] <= '9'){
				num2 = line[i] - '0';
				break;
			}
		}
			
		// Add the digit to the running total
		total += num1*10 + num2;
	}
	
	// Free the dynamically-allocated variable and close the file
	free(line);
	fclose(text_in);

	// Print the total
	printf("The total is: %d\n", total);
	return 0;
}