"""
 ============================================================================
 Name: Ryan Russell
 UVicID: V00873387
 Created: Nov. 6th, 2017
 Last Updated: Nov. 12th, 2017
 Assignment: SENG 265 Assignment 3 Part A
 File name: word_count.py
 Description: This program, given the correct input commands, counts 
 how many words there are of each length in a text file, sorts the 
 word lengths in descending order of frequency, and displays the unique
 words for each word length in alphanumeric order.
 ============================================================================
 """

import sys
import os

# This function takes a list of words that are sorted alphanumerically as
# input and prints the ones that correspond the given word length, 
# separated by commas and an "and" 
def print_alphanumerically(sorted_words, length):

	new_words = [] # A new list that stores only the words of a specific length

	# Creates a new list of words that are of the length "length"
	for word in sorted_words:
		if len(word) == length:
			new_words.append(word)

	# Prints the formatted output for the alphanumerically sorted words 
	print(" (words: ", end="")
	if len(new_words) > 2:
		for word in new_words[:-2]:
		  	print("\"" + word + "\"", end=', ')
		print("\"" + new_words[-2] + "\"" + " and " + "\"" +  new_words[-1] + "\"" + ")")
	elif len(new_words) == 2:
		print("\"" + new_words[0] + "\"" + " and " + "\"" + new_words[1] + "\"" + ")")
	elif len(new_words) == 1:
		print("\"" + new_words[0] + "\"" + ")")


def main():

	tokenizers = ".,;()"     # A string of accepted characters in the input file
	unique_words = []        # A list to contain all unique words
	cur_word = ""            # A string to contain the current word being read
	current_length = 0       # The current word length when looping through each frequency
	start = 0                # Where the list should start looking for the index

	# Initialize the occurence of each command line tag to 0
	infile = 0
	sort = 0
	print_words = 0
	text_file = 0

	# Checks the command line input for the --infile, --sort, and --print-words tags 
	# as well as a txt file and sets a boolean value to true if the tags are found
	for i in sys.argv:
		if i == '--infile':
			infile = i
		if i == '--sort':
			sort = i
		if i == '--print-words':
			print_words = i
		if '.txt' in i or '.csv' in i:
			text_file = i

	# Opens the input file found in the command line input
	if text_file == 0:
		print("No input file found.")
		return
	elif os.path.exists(text_file):
		 file = open(text_file, "r")
	else:
		print("The specified input file does not exist in this directory.")
		return

	# Reads all text from the file, converts every character to lower case,
	# and inserts it into a string called text
	text = file.read().lower()

	total_chars = len(text)            # The total characters in the input file
	counts = list(range(total_chars))  # A list of all possible word count in the input file 
	frequencies = [0] * total_chars    # A list contain the frequency of each word length

	# Looks at every character in text one by one and tokenizes text
	# into separate strings based on the tokenizers string
	# NOTE: This is note modularized into its own function because there would be multiple 
	# return values and it is only required to be done once in the program
	for letter in text:
		if not letter.isspace() and letter not in tokenizers:
			cur_word = cur_word + letter 
		elif letter.isspace() and len(cur_word) > 0:
			frequencies[len(cur_word)] += 1
			if cur_word not in unique_words:
				unique_words.append(cur_word)
			cur_word = ""

	sorted_words = sorted(unique_words)   # Sorts the list of unique words alphanumerically

	# All printing is included in this code block, utilizing the print_alphanumerically function
	if infile:
		if sort:

			# Searches through the list of possible word counts, then compares to 
			# the frequencies, and prints the sorted, formatted output
			for i in reversed(counts):
				for frequency in frequencies:
					if frequency > 0 and frequency == i:
						print("Count[" + str(frequencies.index(frequency, start)).rjust(2, '0') + "]=" + str(frequency).rjust(2, '0') + ";", end="")
						if print_words:
							current_length = frequencies.index(frequency, start)
							print_alphanumerically(sorted_words, current_length)
						else:
							print()
					start += 1  # Increments the starting location for the index search
				start = 0
		else:

			# Searches through the list of frequencies and prints the formatted output
			for frequency in frequencies:
				if frequency > 0:
					print("Count[" + str(frequencies.index(frequency, start)).rjust(2, '0') + "]=" + str(frequency).rjust(2, '0') + "; ", end="") 
					if print_words:
						current_length = frequencies.index(frequency, start)
						print_alphanumerically(sorted_words, current_length)
					else:
						print()
				start += 1  # Increments the starting location for the index search
	else:
		print("No command was entered to prompt the word_count program.")

	file.close()

if __name__ == "__main__":
	main()