#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE* fp = fopen(argv[1], "r");
	char buffer[1024];
	int result = 0;

	if (fp == NULL) {
		printf("Error: error opening file\n");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char* first = NULL;
		char* last = NULL;
		char temp[3];

		for (int i = 0; i < strlen(buffer); i++) {
			if (isdigit(buffer[i]) == 0) {
				continue;
			}

			if (first == NULL) {
				first = &buffer[i];
			}

			last = &buffer[i];
		}

		if (first == NULL || last == NULL) {
			printf("Error: error on string: %s", buffer);
		}

		temp[0] = *first;
		temp[1] = *last;
		result += atoi(temp);
	}

	printf("%d\n", result);

	fclose(fp);
	return 0;
}

