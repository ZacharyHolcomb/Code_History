/*
  Authors (group members): Brian Massino, Brandon DuPree, Zachary Holcomb, Edouard Gruyters
  Email addresses of group members: bmassino2017@my.fit.edu, bdupree2017@my.fit.edu, zholcomb2017@my.fit.edu, egruyters2017@my.fit.edu
  Group name:

  Course: CSE 2010
  Section: 4

  Description of the overall algorithm:
 
	Utilizes a linkedlist holding 26 Nodes, A-Z. Each Node has another linkedlist that holds everyword that starts with the corresponding origins node letter ie node A 
	will hold all words starting with the letter A. The initial guess is made based on word popularity alone, as more letters are inputted the search window narrows so
	that everyword starts with the inputted letters. If Tot is inputted, only words starting with Tot will be searched and the moment it goes to Tos, the search will 
	stop. Words that are completely inputted will increment the popularity counter of that word and if it is not an already known word it will be added to the list. All
	words are read from the words.txt file and additional words and word popularity counts are based on old_tweet files.
*/


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "smartWord.h"

#define STR_LEN 100

/*
  Description for each function, parameter, and return value (if any)
 */
typedef struct word_node {
	unsigned short pop; //Word's Popularity
	struct word_node *next; //Pointer to the next word
	char *word; //Holds the word
}word_node;

word_node *list[26]; //Array to hold the different linked list for each letter
char target[STR_LEN] = ""; //Holds the word that is being typed in
word_node *start = NULL; //Pointer to the start of the first occurence of the word with the same first letters
word_node *guess[3]; //Holds 3 pointers to the 3 most frequently used words

// initialize smartWord by process a file of English dictionary words
void initSmartWord(char *wordFile) {
	FILE *words = fopen(wordFile, "r"); //Open the word.txt file
	char str[STR_LEN] = "\0"; //Temp to hold the word
	int index = 0; //Index to assign the words to the letter list
	char currentChar = 'a'; //Current character of which list to add the word to
	word_node *current = NULL; //Hold the last used word_node
	while(fscanf(words, "%s", str) != EOF) { //Cycle through all the word
		if(str[0] > 64 && str[0] < 91) { //If the first letter of string is uppercase
			str[0] += 32; //Convert capital 
		}
		if(str[0] > currentChar) { //If the first character of the word is greater then the current char
			index++; //Increment the index
			currentChar++; //Increment the character
		}
		
		word_node *new_word = (word_node *)malloc(sizeof(word_node)); //Malloc the word
		new_word->pop = 0; //Set the popularity
		new_word->next = NULL; //
		new_word->word = (char*) malloc(sizeof(char)*strlen(str));
		strcpy(new_word->word, str); //Copy the word to created struct
		
		if(list[index] == NULL) { //Check if the start of the list is empty
			list[index] = new_word; //Add the node to the start of the list
			current = list[index]; //Upadate the current to the start of the list
		} else {
			current->next = new_word; //Add the node to the end of the list
			current = current->next; //Move to the end of the list
		}
	}
	
	//DEBUG
	/*for(int i = 0; i < 26; i++) {
		printf("\n---%c---\n", currentChar);
		word_node *current = list[i];
		while(current != NULL) {
			printf("%s ", current->word);
			current = current->next;
		}
	}*/
	
}


// process old messages from oldMessageFile
void procOldMsgSmartWord(char *oldMessageFile) {
	FILE *oldMessage = fopen(oldMessageFile, "r"); //Open the old messages
	char str[STR_LEN] = "\0"; //Holds the copied word
	while(fscanf(oldMessage, "%s", str) != EOF) { //Loop through all the words in the file
		for(int i = 0; i < strlen(str); i++) { //Cycle through the letters
			if(str[i] > 64 && str[i] < 91) { //If the first letter of string is uppercase
				str[i] += 32; //Convert capital 
			}
		}
		for(int i = 0; i < strlen(str); i++) { //Cycle through the letters
			if(str[i] > 122 || str[i] < 97) { //Check if letter is not in the alphabet
				for(int j = i; j < strlen(str); j++) { //Loop through the remaining letters in the string
					str[j] = str[j + 1]; //Shift all the letter to the left
				}
				i--; //Decrement to prevent any non-alphabetical characters from being skipped
			}
		}
		int flag = 1; //Flag to check if the word exists within our dictionary
		if(str[0] - 97 > 0) { //Check if the word exists
			word_node *current = list[str[0] - 97]; //Get the start of the appropriate list of words.
			while(current != NULL && flag) { //Loop through all the words and whilest the flag is true
				if(strcmp(str, current->word) == 0) { //Check if the word from the file is equal to the word in the dictionary
					current->pop++; //Increase the words popularity
					flag = 0; //Set the flag to false
				}
				current = current->next; //Move to the next word
			}
		}
		if(flag) { //If the flag is still true
			if(str[0] - 97 > 0) { //Check if the word exists
				word_node *new_word = (word_node *)malloc(sizeof(word_node)); //Malloc the word
				new_word->pop = 1; //Set the popularity
				new_word->next = NULL; //Set the next to NULL
				new_word->word = (char*) malloc(sizeof(char)*strlen(str)); //Create a word struct
				strcpy(new_word->word, str); //Copy the word to created struct
				
				if(strcmp(list[str[0] - 97]->word, str) > 0) { //Add to the head of the list
					new_word->next = list[str[0] - 97]; //Set the word next to the head
					list[str[0] - 97] = new_word; //Set the list head to the word
				} else { //Insert to the list
					word_node *current2 = list[str[0] - 97]; //Get the start of the appropriate list of words.
					while(current2->next != NULL) { //Loop through the words
						if(strcmp(current2->next->word, str) > 0) { //Check if the word is less than the current word
							new_word->next = current2->next; //Insert the word into the list
							current2->next = new_word;
							break; //End the while loop
						}
						current2 = current2->next; //Move to te next word
					}
				}
			}
		}
	}
	
	//DEBUG
	/*for(int i = 0; i < 26; i++) {
		printf("\n---%c---\n", (i + 97));
		word_node *current = list[i];
		while(current != NULL) {
			if(current->pop > 0) {
				printf("%s %d\n", current->word, current->pop);
			}
			current = current->next;
		}
	}*/
}

// Given:
//   letter: letter typed in by the user (a-z, A-Z)
//   letterPosition:  position of the letter in the word, starts from 0
//   wordPosition: position of the word in a message, starts from 0
// Return via a parameter:
//   guesses: NUM_GUESSES (3) word guesses (case-insensive comparison is used) 

void guessSmartWord(char letter, int letterPosition, int wordPosition, char guesses[NUM_GUESSES][MAX_WORDLEN+1]) {
	//printf("letter: %c\nletterPosition: %d\nwordPosition: %d\n", letter, letterPosition, wordPosition);
	for(int i = 0; i < 3; i++) { //Go through all the guesses
		guess[i] = NULL; //Clear the guess
	}
	
	if(letter > 64 && letter < 91) { //If the first letter of string is uppercase
		letter += 32; //Convert capital 
	}
	target[letterPosition] = letter; //Add the letter to the word
	
	if(start == NULL) { //Check if this is the first letter
		start = list[letter - 97]; //Get the start of the appropriate list of words
		word_node *current = start; //Get the start of the list of words
		while(current != NULL) { //Loop through the list of word
			//printf("%s |compare| %s\n", current->word, target);
				if(guess[0] == NULL) { //Check if the first guess is empty
					guess[0] = current; //Insert the word into the first guess
				} else if(guess[1] == NULL) { //Check if the second guess is empty
					guess[1] = current; //Insert the word into the second guess
				} else if(guess[2] == NULL) { //Check if the third guess is empty
					guess[2] = current; //Insert the word into the third guess
				} else if(guess[0]->pop < current->pop) { //Check if the first guesses popularity is less than the current's popularity
					guess[0] = current; //Set the first guess to the word
				} else if(guess[1]->pop < current->pop) { //Check if the second guesses popularity is less than the current's popularity
					guess[1] = current; //Set the second guess to the word
				} else if(guess[2]->pop < current->pop) { //Check if the third guesses popularity is less than the current's popularity
					guess[2] = current; //Set the third guess to the word
			}
			current = current->next; //Move to the next word
		}
		
	} else {
		word_node *current = start; //Get the start of the list
		while(strncmp(current->word, target, letterPosition + 1) < 0) { //Loop until we reached the first word with equal letters
			current = current->next; //Move to the next word
		}
		start = current; //Set the start to the current word
		
		while(current != NULL && strncmp(current->word, target, letterPosition + 1) == 0) { //Loop through the words and check if the words have the same letters
			//printf("%s |compare| %s | %d\n", current->word, target, strncmp(current->word, target, letterPosition + 1));
				if(guess[0] == NULL) { //Check if the first guess is empty
					guess[0] = current; //Insert the word into the first guess
				} else if(guess[1] == NULL) { //Check if the second guess is empty
					guess[1] = current; //Insert the word into the second guess
				} else if(guess[2] == NULL) { //Check if the third guess is empty
					guess[2] = current; //Insert the word into the third guess
				} else if(guess[0]->pop < current->pop) { //Check if the first guesses popularity is less than the current's popularity
					guess[0] = current; //Set the first guess to the word
				} else if(guess[1]->pop < current->pop) { //Check if the second guesses popularity is less than the current's popularity
					guess[1] = current; //Set the second guess to the word
				} else if(guess[2]->pop < current->pop) { //Check if the third guesses popularity is less than the current's popularity
					guess[2] = current; //Set the third guess to the word
			}
			current = current->next; //Move to the next word
		}
	}
	
	// just to show how to return 3 (poor) guesses
	if(guess[0] != NULL) { //Check if the first guess exists
		//printf("%s\n", guess[0]->word);
		strcpy(guesses[0], guess[0]->word);
	} else {
		//printf("NON\n");
		strcpy(guesses[0], "a"); //Give a spaceholder suggestion
	}
	if(guess[1] != NULL) { //Check if the second guess exists
		//printf("%s\n", guess[1]->word);
		strcpy(guesses[1], guess[1]->word);
	} else {
		//printf("NON\n");
		strcpy(guesses[1], "b"); //Give a spaceholder suggestion
	}
	if(guess[2] != NULL) { //Check if the third guess exists
		//printf("%s\n", guess[2]->word);
		strcpy(guesses[2], guess[2]->word);
	} else {
		//printf("NON\n");
		strcpy(guesses[2], "c"); //Give a spaceholder suggestion
	}
}





// feedback on the 3 guesses from the user
// isCorrectGuess: true if one of the guesses is correct
// correctWord: 3 cases:
// a.  correct word if one of the guesses is correct
// b.  null if none of the guesses is correct, before the user has typed in 
//            the last letter
// c.  correct word if none of the guesses is correct, and the user has 
//            typed in the last letter
// That is:
// Case       isCorrectGuess      correctWord   
// a.         true                correct word
// b.         false               NULL
// c.         false               correct word

// values for bool: true (1), false (0)  
void feedbackSmartWord(bool isCorrectGuess, char *correctWord) {
	if(isCorrectGuess && correctWord != NULL) { //Check if we guessed the right word
		if(guess[0] != NULL && strcmp(guess[0]->word, correctWord) == 0) { //Check if the first guess is equal to the correct word
			guess[0]->pop++; //Increase the popularity of the word
		} else if(guess[1] != NULL && strcmp(guess[1]->word, correctWord) == 0) { //Check if the second guess is equal to the correct word
			guess[1]->pop++; //Increase the popularity of the word
		} else if(guess[2] != NULL && strcmp(guess[2]->word, correctWord) == 0) { //Check if the third guess is equal to the correct word
			guess[2]->pop++; //Increase the popularity of the word
		}
		for(int i = 0; i < STR_LEN; i++) { //Loop through all the letters of the target string
			target[i] = '\0'; //Reset the letters in the target string
		}
		start = NULL; //Set the start to NULL
	} else if(!isCorrectGuess && correctWord != NULL) { //Check we have completed the word and we haven't guessed it
		for(int i = 0; i < STR_LEN; i++) { //Loop through all the letters of the target string
			target[i] = '\0'; //Reset the letters in the target string
		}
		start = NULL; //Set the start to NULL
	}
}

