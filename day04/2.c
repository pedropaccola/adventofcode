#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define SCRATCH_SIZE 1024

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

void parse(int** numbers, char* input) {
	int i = 0;
	int pointer = 0;

	while (input[pointer] != '\0') {
		if (input[pointer] == ' ') {
			pointer++;
			continue;
		} else if (isDigit(input[pointer])) {
			int length = 0;
			while (isDigit(input[pointer])) {
				length++;
				pointer++;
			}

			numbers[i] = malloc(sizeof(int) * 1);
			char* temp = malloc(sizeof(char) * (length + 1));
			if (numbers[i] == NULL || temp == NULL) {
				perror("Error");
			}

			strncpy(temp, &input[pointer - length], length);
			temp[length] = '\0';
			*numbers[i] = atoi(temp);
			free(temp);

			i++;
		} else {pointer++;}
	}
	return;
}

int findWinners(int** winNumbers, int** myNumbers) {
	int result = 0;
	int i = 0;

	for (int i = 0; myNumbers[i] != NULL; i++) {
		for (int j = 0; winNumbers[j] != NULL; j++) {
			if (*myNumbers[i] == *winNumbers[j]) {
				result++;
			}
		}
	}

	return result;
}

void freeMemory(int** array) {
	int i = 0;
	while(array[i] != NULL) {
		free(array[i]);
		i++;
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		perror("Error");
		return 1;
	}

	FILE* fp = fopen(argv[1], "r");
	char fBuffer[BUFFER_SIZE];
	int scratchCount[BUFFER_SIZE]; 
	for (int i = 0; i < BUFFER_SIZE; i++) {scratchCount[i] = 1;} // Starts with one of each ticket
	int card = 0;  // One card per line - to keep track
	int result = 0;

	if (fp == NULL) {
		perror("Error");
		return 1;
	}

	while (fgets(fBuffer, sizeof(fBuffer), fp) != NULL) {
		printf("%s", fBuffer);
		
		// for the strtok_r function
		const char* delim = ":|\n";
		char* token;
		char* token_save;

		int* winNumbers[SCRATCH_SIZE] = {NULL};
		int* myNumbers[SCRATCH_SIZE] = {NULL};

		token = strtok_r(fBuffer, delim, &token_save); // Card number, ignored

		token = strtok_r(NULL, delim, &token_save);
		parse(winNumbers, token);

		token = strtok_r(NULL, delim, &token_save);
		parse(myNumbers, token);

		int cardResult = findWinners(winNumbers, myNumbers); // how many subsequent tickets will be incremented 
		for (int i = 0; i < cardResult; i++) {
			scratchCount[card + i + 1] += scratchCount[card];
		}

		result += scratchCount[card];
		printf("Card: %d, Card result: %d, amount of cards: %d, Result so far: %d\n",card+1, cardResult, scratchCount[card], result);

		card++;
		freeMemory(winNumbers);
		freeMemory(myNumbers);
	}

	printf("Sum: %d\n", result);

	fclose(fp);
	return 0;
}
