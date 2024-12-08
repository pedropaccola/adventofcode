#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_DIGITS 9
 
typedef struct {
	 char* word;
	 int value;
} lookup_t;

const lookup_t gTable[] = {
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

int match(char* string) {
	for (int i = 0; i < NUM_DIGITS; i++) {
		if (strncmp(gTable[i].word, string, strlen(gTable[i].word)) == 0) {
			return gTable[i].value;		
		}
	}
	return -1;
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
		int first = -1;
		int last = -1;

		for (int i = 0; i < strlen(buffer); i++) {
			if (isDigit(buffer[i]) != 0) {
				if (first == -1) {
					first = buffer[i] - '0';
				}

				last = buffer[i] - '0';
			} else {
				int matchnum = match(&buffer[i]);
				if (matchnum == -1) {
					continue;
				}

				if (first == -1) {
					first = matchnum;
				}

				last = matchnum;
			}

		}
			
		if (first == -1 || last == -1) {
			printf("Error: error on string %s\n", buffer);
			continue;
		}

		result += (first * 10) + last;
	}

	printf("%d\n", result);

	fclose(fp);
	return 0;
}
