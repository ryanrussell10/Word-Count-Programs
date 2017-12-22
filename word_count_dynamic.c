/*
 ============================================================================
 Name: Ryan Russell
 UVicID: V00873387
 Created: Oct. 23rd, 2017
 Last Updated: Nov. 1st, 2017
 Assignment: SENG 265 Assignment 2
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
#include <assert.h>


/* The following functions are adapted from the SENG 265  */
/* lecture slides on dynamic memory and linked lists.     */

// The creation of the Node datatype (for linked lists) using a struct
typedef struct Node Node;
struct Node { 
	char *word; 
	int length;
	Node *next; 
};


// The creation of the count_node datatype (for linked lists) using a struct
typedef struct count_node count_node;
struct count_node { 
	int frequency; 
	int length;
	count_node *next;
};


// The emalloc function attempts to allocate memory with size n and 
// returns an error message if failure occurs
void *emalloc (size_t n) { 
	void *p; p = malloc(n);
	if (p == NULL) { 
		fprintf(stderr, "malloc of %zu bytes failed", n); 
	 	exit(1); 
	} 
	return p; 
}


// The string_allocator function allocates memory for a string given a string as input
char *string_allocator (char *input) { 
	char *copy; 
	assert (input != NULL); 
	copy = (char *)malloc(sizeof(char) * strlen(input) + 1);
	if (copy == NULL) { 
		fprintf(stderr, "error in string_allocator");
		exit(1);
	} 
	strncpy(copy, input, strlen(input)+1); 
	return copy; 
}


// The new_item function takes input for the word and length of the new item. Then, it 
// allocates space for and creates a new item that can be added to a list 
Node *new_item (char *word, int length) { 
	Node *new_node; 
	new_node = (Node *) emalloc(sizeof(Node)); 
	new_node->word = word;
  	new_node->length = length; 
 	new_node->next  = NULL; 
 	return new_node; 
}


// The new_count function takes input for the frequency and length of the new count list node.
// Then, it allocates space for and creates a new count node that can be added to a list 
count_node *new_count(int frequency, int length) { 
	count_node *new_count_node; 
	new_count_node = (count_node *) emalloc(sizeof(count_node)); 
	new_count_node->frequency = frequency;
  	new_count_node->length = length; 
 	new_count_node->next  = NULL; 
 	return new_count_node; 
}


// The add_node function adds a node to the front of a list using the list pointer
Node *add_node (Node *listp, Node *newp) {
	newp->next = listp; 
	return newp; 
}


// The add_count function adds a count_node to the front of a list using the list pointer
count_node *add_count (count_node *listp, count_node *newp) {
	newp->next = listp; 
	return newp; 
}


// The free_nodes functions frees all the memory allocated to a list of nodes given by listp
void free_nodes (Node *listp) { 
	Node *next; 
	for ( ; listp != NULL; listp = next ) {
		next = listp->next;  
		free(listp->word);
		free(listp);  
	} 
}


// The free_counts functions frees all the memory allocated to a list of counts given by listp
void free_counts (count_node *listp) {
	count_node *next;
	for ( ; listp != NULL; listp = next ) {
		next = listp->next;  
		free(listp);  
	} 
}

/* End of linked list and dynamic memory functions from lecture slides. */


int main (int argc, char *argv[]) {

	// Loop counters
	int i = 0; 
	int j = 0;


	// Integer variables for determining 
	// the index of inputted commands
	int infile = 0;
	int sort = 0;
	int print_words = 0;
	int text_file = 0;

	// Checks the command line input for the --infile, --sort, and --print-words tags 
	// as well as a txt file and sets a boolean value to true if the tags are found
	for (i = 0; i < argc; i++) {
		if (strstr(argv[i], ".txt") != NULL) text_file = i;
		if (strcmp(argv[i], "--infile") == 0) infile = i;
		if (strcmp(argv[i], "--sort") == 0) sort = i;
		if (strcmp(argv[i], "--print-words") == 0) print_words = i;
	}


	// Initializes the input FILE pointer to a file that may now be opened
	FILE* ifp;

	// Opens the input file found in the command line input
	ifp = fopen(argv[text_file], "r");
	if (ifp == NULL) {
		fprintf(stderr, "Cannot open input file\n");
		return 0;
	}


	// Using fseek and ftell, the total number of characters in the input file is determined
	fseek(ifp, 0, SEEK_END);
	int total_chars = ftell(ifp) + 1;
	fseek(ifp, 0, SEEK_SET);


	/* The following variable and array declarations are only for reading from the file */

	// An array that stores the characters of the current word
	char cur_word[total_chars];

	// Complete unique word count for the input file
	int word_count = 0;

	// Complete total word count for the input file
	int total_word_count = 0;

	// The amount of characters in the current string
	int char_count = 0; 

	// The current character returned from fgetc
	char current_char = ' ';

	// Boolean variable that indicates whether or not the current word is a duplicate
	int duplicate = 0;

	/* File reading declarations end here */

	
	// Used to store the frequency of a certain word length for printing purposes
	int freq = 0;

	// Used to store the greatest frequency of a word length in a given input file
	int max_freq = 0;


	if (infile) {
		
		/* The following code block reads and tokenizes using a buffer array and a linked list */

		// List head declarations 
		Node* unique_words_head = NULL;
		Node* all_words_head = NULL;
		count_node* count_head = NULL;

		while (current_char != EOF) {
					
			// Returns the next character from the input file
			current_char = fgetc(ifp);

			if (isalpha(current_char) || isdigit(current_char)) {

				// Convert each character to lower case
				current_char = tolower(current_char);
				cur_word[char_count] = current_char;

				char_count++;

			} else if ( (current_char == ' ') || (current_char == '\n') || (current_char == EOF) ) {

				total_word_count++;

				// Ensures that the current word is capped with a null terminator
				cur_word[char_count] = '\0';

				// Adds a new item to the list of all the words in the input file
				Node *new_node = new_item(string_allocator(cur_word), char_count);
				all_words_head = add_node(all_words_head, new_node);

				// Determines whether or not the current word is unique
				Node *cur = unique_words_head;
				while (cur != NULL) {    
					if (strcmp(cur->word, cur_word) == 0) {
						duplicate = 1;
						break;
					}
					cur = cur->next; 
				} 	

				// If the current word is not a duplicate, add a new node to the 
				// unique words list to store the word and its length. Base cases: if the list is empty 
				// (just add the node to the front), and if the cur_word has a lower alphanumeric value 
				// than the head of the list (make the node with cur_word the new list head).
				if (duplicate == 0) {

					if (unique_words_head == NULL) {
						Node *new_node = new_item(string_allocator(cur_word), char_count);
						new_node->next = NULL;
						unique_words_head = new_node;
					} else if ((strcmp(unique_words_head->word, cur_word) > 0)) {
						Node *new_node = new_item(string_allocator(cur_word), char_count);
						new_node->next = unique_words_head;
						unique_words_head = new_node;
					} else {
						Node* lp = unique_words_head;

						while (lp->next != NULL) {
							if (strcmp(lp->word, cur_word) < 0 && strcmp(lp->next->word, cur_word) > 0) {
								break;
							} else {
								lp = lp->next;
							}
						}

						if (strcmp(lp->word, cur_word) < 0) {
							Node *new_node = new_item(string_allocator(cur_word), char_count);
							new_node->next = lp->next;
							lp->next = new_node;
						}
					}

					word_count++;
				}

				for (j = 0; j < char_count; j++) 
					cur_word[j] = 0;

				// Resets the count and duplicate flag
				char_count = 0;
				duplicate = 0;

			} // End of if statements
		} // End of while loop

		/* The read and tokenize code block ends */

		
		// Initializes multiple cur pointers that point to the start of lists for list traversal
		Node *cur = unique_words_head;
		Node *cur2 = unique_words_head;
		Node *cur3 = all_words_head;
		count_node* cur4 = count_head;


		// This code block looks through the all_words list for each possible word length
		// in the input file and counts the frequency of each word length. If the frequency of a 
		// certain word length is greater than zero, a count_node in the count list is created.
		freq = 0;
		for (i = total_chars; i > 0 ; i--) {
			while (cur3 != NULL) {
				if (i == strlen(cur3->word)) {
					freq++;
				}
				cur3 = cur3->next;
			}
			if (freq > 0) {
				count_node *new_count_node = new_count(freq, i);
				count_head = add_count(count_head, new_count_node);
			}
			if (freq > max_freq) {
				max_freq = freq;
			}
			freq = 0;
			cur3 = all_words_head;
		}


		if (sort) { /* Code block for --sort tag begins here */

			cur4 = count_head;
			for (i = max_freq; i > 0; i--) {
				while (cur4 != NULL) {
					if (i == cur4->frequency) {
						printf("Count[%.2d]=%.2d;", cur4->length, cur4->frequency);

						if (print_words) { /* Code block for --print-words tag begins here */

							cur2 = unique_words_head;
							printf(" (words:");
							while (cur2 != NULL) {
								if ((strlen(cur2->word) > 0) && (cur2->length == cur4->length)) {
									printf(" \"%s\"", cur2->word);
								}
								cur2 = cur2->next;
							}
							printf(")\n");
							cur2 = unique_words_head;
									
						} else { /* Code block for --print-words tag ends here */
							printf("\n");
						}

					}
					cur4 = cur4->next;
				}
				cur4 = count_head;
			}

		 /* Code block for --sort tag ends here */
		} else { /* Code block for --infile tag begins here */
			
			cur = all_words_head;
			freq = 0;
			for (i = 1; i <= total_chars; i++) {
				while (cur != NULL) {
					if (strlen(cur->word) == i) {
						freq++;
					}
					cur = cur->next;
				}
				if (freq > 0) {
					printf("Count[%.2d]=%.2d;", i, freq);

					if (print_words) { /* Code block for --print-words tag begins here */

						cur2 = unique_words_head;
						printf(" (words:");
						while (cur2 != NULL) {
							if ((strlen(cur2->word) > 0) && (cur2->length == i)) {
								printf(" \"%s\"", cur2->word);
							}
							cur2 = cur2->next;
						}
						printf(")\n");
						cur2 = unique_words_head;
								
					} else { /* Code block for --print-words tag ends here */
						printf("\n");
					}

				}
				freq = 0;
				cur = all_words_head;
			}
		} /* Code block for --infile tag ends here */

		// Frees the memory allocated to the list of nodes
		free_nodes(unique_words_head);
		free_nodes(all_words_head);
		free_counts(count_head);

	} else {
		fprintf(stderr, "No command was entered to prompt the word_count program.\n");
	}

	fclose(ifp);

	return 0;
}
