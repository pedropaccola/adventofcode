#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define MAX_CATEGORIES 7

typedef struct {
	long dest;
	long source;
	long length;
} mapping_t;

typedef struct {
	long start;
	long length;
} seeds_t;

mapping_t **maps[MAX_CATEGORIES] = {NULL};
seeds_t seeds[BUFFER_SIZE];
int num_seeds = 0;

int is_digit(char c) 
{
	return (c >= '0' && c <= '9');
}

int parse_file(const char* filename) 
{
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("ERROR: can't open file %s\n", filename);
		return 0;
	}

	char buffer[BUFFER_SIZE];
	int category = -1;
	int mapping = 0;

	while(fgets(buffer, sizeof(buffer), file) != NULL) {
		// Read seeds (always on first line)
		if (strncmp(buffer, "seeds:", strlen("seeds:")) == 0) {
			int pointer = 0;
			int n;
			while (sscanf(buffer + pointer, "%*[^0123456789]%ld %ld%n", &seeds[num_seeds].start, &seeds[num_seeds].length, &n) == 2) {
				pointer += n;
				num_seeds++;
			}
		} else if (strstr(buffer, "-to-")) {
			category++;
			mapping = 0;
		} else if (buffer[0] == '\n') {
			continue;
		} else {
			// +2 to leave room for a null pointer terminator (to iterate through it)
			maps[category] = (mapping_t **)realloc(maps[category], (mapping+2) * sizeof(mapping_t *));
			if (!maps[category]) {
				printf("ERROR: memory allocation for the category failed");
				return 0;
			}

			maps[category][mapping] = (mapping_t *)malloc(sizeof(mapping_t));
			if (!maps[category][mapping]) {
				printf("ERROR: memory allocation for the mapping failed");
				return 0;
			}

			long dest, source, length;
			sscanf(buffer, "%ld %ld %ld", &dest, &source, &length);
			maps[category][mapping]->dest = dest;
			maps[category][mapping]->source = source;
			maps[category][mapping]->length = length;

			mapping++;
		}
	}
	// printf("Seeds: ");
	// for (int i = 0; i < num_seeds; i++) {
	// printf("%ld to %ld - ", seeds[i].start, seeds[i].start + seeds[i].length - 1);
	// }
	// printf("\n");
	// for (int category = 0; category < MAX_CATEGORIES; category++) {
	// 	printf("Category: %d\n", category + 1);
	// 	for (int mapping = 0; maps[category][mapping]; mapping++) {
	// 		printf("Mapping: %d, Dest: %ld, Source: %ld, Length: %ld\n", mapping + 1, maps[category][mapping]->dest, maps[category][mapping]->source, maps[category][mapping]->length);
	// 	}
	// }
	fclose(file);
	return 1;
}

long find_location(long seed) {
	long current = seed;
	for (int category = 0; category < MAX_CATEGORIES; category++) {
		for (int mapping = 0; maps[category][mapping]; mapping++) {
			if (current >= maps[category][mapping]->source &&
				current < maps[category][mapping]->source + maps[category][mapping]->length) {
				// printf("Found mapping for seed %ld. Category %d. Mapping %d. Old value: %ld ", seed, category + 1, mapping + 1, current);
				current += maps[category][mapping]->dest - maps[category][mapping]->source;
				// printf("New value: %ld\n", current);
				break;
			}
		}
	} 
	return current;
}

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (!parse_file(argv[1])) {
		return 1;
	}
	
	long lowest_location = LONG_MAX;
	for (int i = 0; i < num_seeds; i++) {
		printf("%d - range of seeds: %ld to %ld\n",i + 1,seeds[i].start, seeds[i].start + seeds[i].length - 1);
		for (long j = 0; j < seeds[i].length; j++) {
			long result = find_location(seeds[i].start + j);
			if (result < lowest_location) {
				lowest_location = result;
			}
		}
	}

	for (int i = 0; i < MAX_CATEGORIES; i++) {
		for (int j = 0; maps[i][j]; j++) {
			free(maps[i][j]);
		}
		free(maps[i]);
	}

	printf("Result: %ld\n", lowest_location);

	return 0;
}
