#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Find all occurances of sub_str in full_str. Update min and max value and the values of num1 and num2 accordingly
void find_num_chars(const char* full_str, const char* sub_str, int value, int* min, int* max, int* num1, int* num2);

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
	char* num_chars[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}; // Array of numbers in the form of strings
	ssize_t char_read = 0; // Number of characters read by getline()

	// Read in and check each line of the text file
	while((char_read = getline(&line, &line_len, text_in)) != -1){
		int num1 = 0; // Stores the leftmost number
		int num2 = 0; // Stores the rightmost number
		int min_index = -1; // Index of current leftmost number
		int max_index = -1; // Index of current rightmost number

		// Find the leftmost and rightmost numbers, which are written as strings
		for (int i = 0; i < 9; i++){
			find_num_chars(line, num_chars[i], i + 1, &min_index, &max_index, &num1, &num2);
		}

		// Now we must consider the numeric digits

		// We only have to check if a digit exits before the current minimum index
		int loop_end = min_index != -1 ? min_index : char_read;
		for (int i = 0; i < loop_end; i++){
			if (line[i] >= '0' && line[i] <= '9'){ 
				num1 = line[i] - '0';
				break;
			}
		}

		// We only have to check if a digit exists after the current maximum index
		loop_end = max_index != -1 ? max_index : 0;
		for (int i = char_read - 1; i >= loop_end; i--){
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

void find_num_chars(const char* full_str, const char* sub_str, int value, int* min, int* max, int* num1, int* num2){
	
	char* result = strstr(full_str, sub_str); // Stores the pointer to the start of each occurance of the sub_str in full_str
	
	// Find all occurances of sub_str in full_str
	while(result != NULL) {
		int pos = (int)(result - full_str); // The index of the first character of the occurance of sub_str in full_str
		
		// If min has not been updated before (value of -1) or if current index is smaller than
		// previous minimum index, update the index and the value of num1
		if (*min == -1 || pos < *min){
			*min = pos;
			*num1 = value;
		}

		// If max has not been updated before (value of -1) or if current index is larger than
		// previous maximum index, update the index and the value of num2
		if (*max == -1 || pos > *max){
			*max = pos;
			*num2 = value;
		}

		// Check for next occurance of sub_str in full_str
		result = strstr((result + sizeof(char)), sub_str);
	}
	
}