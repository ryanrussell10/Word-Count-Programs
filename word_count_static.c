/*
 ============================================================================
 Name: Ryan Russell
 UVicID: V00873387
 Created: Sep. 25th, 2017
 Last Updated: Oct. 11th, 2017
 Assignment: SENG 265 Assignment 1
 File name: word_count.c
 Description: This program, given the correct input commands, counts 
 how many words there are of each length in a text file, sorts the 
 word lengths in descending order of frequency, and displays the unique
 words for each word length in alphanumeric order.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_SIZE 34
#define MAX_WORDS 750

int main (int argc, char *argv[]) {
 
	// Initializes the input FILE pointer to a file that may now be opened
	FILE* ifp;

	// Integer variables for determining 
	// the index of inputted commands
	int infile = 0;
	int sort = 0;
	int print_words = 0;
	int text_file = 0;

	// An array to store the amount of 
	// words of each length from input file
	int word_lengths[MAX_WORD_SIZE+1] = {0};

	// An array that stores the characters of the current word
	char cur_word[MAX_WORD_SIZE+1] = {0};

	// Complete unique word count for the input file
	int word_count = 0;

	// The amount of characters in the current string
	int char_count = 0; 

	// The current character returned from fgetc()
	char current_char = ' ';

	// A 2-dimensional array that contains the length of each word
	char all_words[MAX_WORDS][MAX_WORD_SIZE];

	// Boolean variable that indicates whether or not the current word is a duplicate
	int duplicate = 0;

	// Loop counters
	int i = 0; 
	int j = 0;
	int k = 0;
	int l = 0;

	// Checks the command line input for the --infile, --sort, and --print-words tags 
	// as well as a txt file and sets a boolean value to true if the tags are found
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], ".txt") != NULL) text_file = i;
		if (strcmp(argv[i], "--infile") == 0) infile = i;
		if (strcmp(argv[i], "--sort") == 0) sort = i;
		if (strcmp(argv[i], "--print-words") == 0) print_words = i;
	}

	// Opens the input file found in the command line input
	ifp = fopen(argv[text_file], "r");
	if (ifp == NULL) {
		printf("Cannot open input file\n");
		return 0;
	}

	if (infile) {
		if (sort) { /* Code block for --sort tag begins here */

				while (current_char != EOF) {
					
					// Returns the next character from the input file
					current_char = fgetc(ifp);

					if (isalpha(current_char) || isdigit(current_char)) {

						// Converts current character to lower case
						current_char = tolower(current_char);
						cur_word[char_count] = current_char;

						char_count++;

					} else if ( (current_char == ' ') || (current_char == '\n') || (current_char == EOF) ) {

						// Increments the counter for words of length char_count by 1
						word_lengths[char_count] = word_lengths[char_count] + 1; 

						// Determines whether or not the current word is unique
						for (l = 0; l < word_count; l++) {
							if (strcmp(all_words[l], cur_word) == 0) {
								duplicate = 1;
								break;
							}
						}

						//If the current word is not a duplicate, insert it into the array of unique words
						if (duplicate == 0) {
							strcpy(all_words[word_count], cur_word);
							word_count++;
						}

						for (j = 0; j < MAX_WORD_SIZE; j++) cur_word[j] = 0;

						// Resets the count and duplicate flag
						char_count = 0;
						duplicate = 0;

					} // End of if statements
				} // End of while loop

				// Prints the formatted output describing the amount of
				// words of each length in descending order of frequency
				for (j = MAX_WORDS; j > 0; j--) {
					for (k = 1; k < MAX_WORD_SIZE+1; k++) {
						if (word_lengths[k] == j) {
							printf("Count[%.2d]=%.2d;", k, word_lengths[k]);


							if (print_words) { /* Code block for --print-words tag begins here */

								int p = 0; //Loop counters
								int q = 0;
								char temp[MAX_WORD_SIZE];

								// Sorts unique words into alphanumeric order using a variation of the selection sort 
								// sorting algorithm. NOTE: this implementation was adapted from multiple
								// websites including the following
								// https://stackoverflow.com/questions/21218798/array-of-strings-selection-sort-c
								// http://www.sanfoundry.com/c-program-sort-names-alphabetical-order/
								for (p = 0; p < word_count; p++) {
									for (q = p + 1; q < word_count; q++) {
										if (strcmp(all_words[p], all_words[q]) > 0) {
											strcpy(temp, all_words[p]);
											strcpy(all_words[p], all_words[q]);
											strcpy(all_words[q], temp);
										}
									}
								}  

								int ordered_word_count = 0;
								char ordered_words[MAX_WORDS][MAX_WORD_SIZE];

								// Stores each unique word in a new ordered array to be operated on before printing
								for (i = 0; i < word_count; i++) {
									if (strlen(all_words[i]) == k) {
										strcpy(ordered_words[ordered_word_count], all_words[i]);
										ordered_word_count++;
									}
								}

								// Prints unique words in alphanumeric order if --print-words tag is found
								printf(" (words:");
								for (l = 0; l < ordered_word_count; l++) {
									if (strlen(ordered_words[l]) == k) {
										if (l == ordered_word_count-1 && ordered_word_count > 1)
											printf(" and");
										printf(" \"%s\"", ordered_words[l]);
										if (l < ordered_word_count-2) 
											printf(",");
									}
								}
								printf(")\n");
							} else printf("\n");
							/* Code block for --print-words tag ends here */

						}
					} // End of nested for loop
				} // End of for loop

			 /* Code block for --sort tag ends here */
		} else { /* Code block for --infile tag begins here */
			
				while (current_char != EOF) {

					// Returns the next character from the input file
					current_char = fgetc(ifp);

					if (isalpha(current_char) || isdigit(current_char)) {

						current_char = tolower(current_char);	// Converts to lower case
						char_count++;

					} else if ( (current_char == ' ') || (current_char == '\n') || (EOF) ) {

						// Increments the counter for words of length char_count by 1
						word_lengths[char_count] = word_lengths[char_count] + 1; 

						// Resets the current character count once a
						// newline or space is encountered
						char_count = 0;

					} // End of if statements
				} // End of while loop

				// Prints the formatted output describing the 
				// amount of words of each length
				for (k = 1; k <= MAX_WORD_SIZE; k++) {
					if (word_lengths[k] > 0) {
						printf("Count[%.2d]=%.2d;\n", k, word_lengths[k]);
					}
				} // End of for loop 

		} /* Code block for --infile tag ends here */
	} else {
		printf("No command was entered to prompt the word_count program.\n");
	}

	fclose(ifp);

	return 0;
}