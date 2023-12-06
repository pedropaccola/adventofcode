#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char* color;
	int quantity;
} lookup_t;

void validateCubes(lookup_t* colors,size_t size, char* color, int cubes) {
	for (int i = 0; i < size; i++) {
		if (strncmp(color, colors[i].color, strlen(colors[i].color)) != 0) {
			continue;
		}
		if (cubes > colors[i].quantity) {
			colors[i].quantity = cubes;
		}
	}
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
		int game = 1;
		lookup_t minCubes[] = {
			{"red", 0},
			{"green", 0},
			{"blue", 0}
		};
		size_t size = sizeof(minCubes) / sizeof(minCubes[0]);

		char* outerToken = strtok_r(buffer, outerDelimiter, &outerSave); //skip game
		outerToken = strtok_r(NULL, outerDelimiter, &outerSave);
		while (outerToken != NULL) {
			char* innerToken = strtok_r(outerToken, innerDelimiter, &innerSave);
			while (innerToken != NULL) {
				int cubes = atoi(innerToken);

				innerToken = strtok_r(NULL, innerDelimiter, &innerSave);

				validateCubes(minCubes, size, innerToken, cubes);

				innerToken = strtok_r(NULL, innerDelimiter, &innerSave);
			}

			outerToken = strtok_r(NULL, outerDelimiter, &outerSave);
		}
		for (int i = 0; i < size; i++) {
			game *= minCubes[i].quantity; 
		}
		result += game;
	}

	printf("%d\n", result);
	fclose(fp);
	return 0;
}
