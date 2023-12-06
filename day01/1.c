#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isDigit(char c) {
	return (c >= '0' && c <= '9');
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
			if (isDigit(buffer[i]) == 0) {
				continue;
			}

			if (first == -1) {
				first =buffer[i] - '0';
			}

			last = buffer[i] - '0';
		}

		if (first == -1 || last == -1) {
			printf("Error: error on string: %s", buffer);
		}


		result += (first * 10) + (last);
	}

	printf("%d\n", result);

	fclose(fp);
	return 0;
}

