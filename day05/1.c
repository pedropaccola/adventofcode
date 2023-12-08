#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		perror("Error");
		return 1;
	}

	FILE* fp = fopen(argv[1], "r");
	char fBuffer[BUFFER_SIZE];
	int result = 0;

	if (fp == NULL) {
		perror("Error");
		return 1;
	}

	while (fgets(fBuffer, sizeof(fBuffer), fp) != NULL) {
		printf("%s", fBuffer);
	}

	printf("Sum: %d\n", result);

	fclose(fp);
	return 0;
}
