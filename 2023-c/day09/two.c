#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

static long values[BUFFER_SIZE][BUFFER_SIZE]; 
static size_t values_length = 0;
static size_t values_sizes[BUFFER_SIZE] = {0};

static long calculate(long*, size_t);
static void print_array(long*, size_t);
static int parse_file(const char*);
static int is_digit(const char);

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
	
	long result = 0;
	for (size_t i = 0; i < values_length; i++) {
		printf("NEW ARRAY: \n");
		long result1 = calculate(&values[i][0], values_sizes[i]);
		result += (long) result1;
		printf("    Result found: %ld\n", result1);
	}

	printf("Result: %ld\n", result);

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
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		int digit = 0;
		int sign = 1;
		size_t i = 0;
		while (buffer[i] != '\n' && buffer[i] != '\0') {
			if (buffer[i] == '-') {
				sign = -1;
				i++;
			}
			if (is_digit(buffer[i])) {
				while (is_digit(buffer[i])) {
					digit = (digit * 10) + (buffer[i] - '0');
					i++;
				}
				values[values_length][values_sizes[values_length]] = digit*sign;
				values_sizes[values_length]++;
				digit = 0;
				sign = 1;
			} else i++; 
		}
		values_length++;
	}

	fclose(file);
	return 1;
}

static long calculate(long* array, size_t size) 
{
	if (size == 1) return array[size-1];
	else if (size == 0) return 0;

	long new_array[size - 1];
	size_t zero_count = (array[0] == 0) ? 1 : 0; 

	printf("    Calculating");
	print_array(array, size);
	printf("\n");

	for (size_t i = 0; i < size - 1; i++) {
		long value = array[i+1] - array[i];
		new_array[i] = value;
		if (value == 0) zero_count++;
	} 

	if (zero_count == size) return 0;
	else return array[0] - calculate(new_array, size - 1);
}

static int is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

static void print_array(long* array, size_t size)
{
	printf("Array: ");
	for (size_t i = 0; i < size; i++) {
		printf("%ld - ", array[i]);
	}
}
