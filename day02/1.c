#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define R_CUBES 12
#define G_CUBES 13
#define B_CUBES 14

typedef struct {
	char* color;
	int value;
} lookup_t;

const lookup_t cTable[] = {
	{"red", R_CUBES},
	{"green", G_CUBES},
	{"blue", B_CUBES},
};

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

int isValidGame(char* string, int cubes) {
	int numColors = sizeof(cTable)/sizeof(lookup_t);
	for (int i = 0; i < numColors; i++) {
		if (strncmp(string ,cTable[i].color, strlen(cTable[i].color)) != 0) {
			continue;
		}
		if (cubes > cTable[i].value) {
			return 0;
		}
	}
	return 1;
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
		const char* outerDelimiter = ":;\n";
		const char* innerDelimiter = ", ";
		char* outerSave = NULL;
		char* innerSave = NULL;
		int game = 0;
		int validGame = 1;

		char* outerToken = strtok_r(buffer, outerDelimiter, &outerSave);
		for (int i = 0; i < strlen(outerToken); i++) {
			if (isDigit(outerToken[i])) {
				game = (game * 10) + (outerToken[i] - 0x30);
			}
		}

		outerToken = strtok_r(NULL, outerDelimiter, &outerSave);
		while (outerToken != NULL) {
			int flag = 0;

			char* innerToken = strtok_r(outerToken, innerDelimiter, &innerSave);
			while (innerToken != NULL && validGame) {
				int cubes = atoi(innerToken);

				innerToken = strtok_r(NULL, innerDelimiter, &innerSave);

				validGame = isValidGame(innerToken, cubes);
				if (!validGame) {
					game = 0;
					break;
				}

				innerToken = strtok_r(NULL, innerDelimiter, &innerSave);
			}

			outerToken = strtok_r(NULL, outerDelimiter, &outerSave);
		}

		result += game;
	}

	printf("%d\n", result);
	fclose(fp);
	return 0;
}
