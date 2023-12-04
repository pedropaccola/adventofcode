#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_DIGITS 9
 
typedef struct {
	 char word[16];
	 int value;
} DigitWord;

const DigitWord digitWords[] = {
	{"one", 1},
	{"two", 2},
	{"three", 3},
	{"four", 4},
	{"five", 5},
	{"six", 6},
	{"seven", 7},
	{"eight", 8},
	{"nine", 9}
};

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

int KMPSearch(char* pattern, char* text, int reverse) {
	int M = strlen(pattern);
	int N = strlen(text);
	int result = -1;

	if (M == 0) {
		return 0;
	}

	int lps[M];
	lps[0] = 0;

	int prevLPS = 0;
	int i = 1;

	while (i < M) {
		if (pattern[i] == pattern[prevLPS]) {
			lps[i] = prevLPS + 1;
			prevLPS++;
			i++;
		} else {
			if (prevLPS != 0) {
				prevLPS = lps[prevLPS - 1];
			} else {
				lps[i] = 0;
				i++;
			}
		}
	}

	i = 0;
	int j = 0;

	while (i < N) {
		if (text[i] == pattern[j]) {
			i++;
			j++;
			if (j == M) {
				if (reverse == 0) {
					return i - j;
				} else {
					result = i - j;
				}
			}
		} else {
			if (j == 0) {
				i++;	
			} else {
				j = lps[j-1];
			}
		}
	}
	return result;
}

int main(int argc, char* argv[]) {
	FILE* fp = fopen(argv[1], "r");
	char buffer[1024];
	int result = 0;

	if (fp == NULL) {
		printf("Error: error opening file\n");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char* firstDigit = NULL;
		char* lastDigit = NULL;
		char* firstAlpha = NULL;
		char* lastAlpha = NULL;
		char temp[2];

		for (int i = 0; i < strlen(buffer); i++) {
			if (isDigit(buffer[i]) == 0) {
				continue;
			}

			if (firstDigit == NULL) {
				firstDigit = &buffer[i];
			}

			lastDigit = &buffer[i];
		}
			
		if (firstDigit == NULL || lastDigit == NULL) {
			printf("Error: error on string %s\n", buffer);
			continue;
		}

		// for (int i = 0; i < NUM_DIGITS; i++) {
		// 	
		// }

		temp[0] = *firstDigit;
		temp[1] = *lastDigit;
		result += atoi(temp);
	}

	printf("%d\n", result);

	fclose(fp);
	return 0;
}
