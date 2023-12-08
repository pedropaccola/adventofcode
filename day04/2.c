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
			if (*myNumbers[i] != *winNumbers[j]) {
				continue;
			}
			if (result > 0) {
				result *= 2;
			} else {
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
	FILE* fp = fopen(argv[1], "r");
	char fBuffer[BUFFER_SIZE];
	int result = 0;
	int card = 0; 

	if (fp == NULL) {
		perror("Error");
		return 1;
	}

	while (fgets(fBuffer, sizeof(fBuffer), fp) != NULL) {
		printf("%s", fBuffer);
		const char* delim = ":|\n";
		char* token;
		char* token_save;
		int* winNumbers[SCRATCH_SIZE] = {NULL};
		int* myNumbers[SCRATCH_SIZE] = {NULL};
		card++;

		token = strtok_r(fBuffer, delim, &token_save); // Card number, ignored

		token = strtok_r(NULL, delim, &token_save);
		parse(winNumbers, token);

		token = strtok_r(NULL, delim, &token_save);
		parse(myNumbers, token);

		int cardResult = findWinners(winNumbers, myNumbers);

		result += cardResult;
		printf("Card result: %d, Result so far: %d\n", cardResult, result);

		freeMemory(winNumbers);
		freeMemory(myNumbers);
	}

	printf("Sum: %d\n", result);

	fclose(fp);
	return 0;
}
