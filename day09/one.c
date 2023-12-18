#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

static int parse_file(const char*);

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (!parse_file(argv[1])) {
		perror("ERROR: fail to parse file");
		return 1;
	}
	

	int result = 0;
	printf("Result: %d\n", result);

	return 0;
}

static int parse_file(const char* filename) 
{
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("ERROR: can't open file %s\n", filename);
		return 0;
	}

	char buffer[BUFFER_SIZE];
	while(fgets(buffer, sizeof(buffer), file) != NULL) {

	}

	fclose(file);
	return 1;
}
