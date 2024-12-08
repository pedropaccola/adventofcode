#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
	S(T) = V * T		-> S(0) = 0
	S(T) = (V+t) * (T-t)	-> considering V = 0 if t = 0. Also t >= 0 and t <= T
	S(T) = t * (T-t)
	S(T) = -t^2 + Tt	-> Example: S(7) = -t^2 + 7t 

	Critical value (inversion point) happens when the derivative = 0;
	S'(T) = -2t + T		-> S'(T) = 0, then
	0 = -2t + T
	t = T/2

	Substituting in the original function
	S(T) = -(T/2)^2 + T(T/2)  
	S(T) = -T^2/4 + T^2/2
	S(T) = T^2/4		-> S(7) = 12.25  biggest integer is 12

	For a given S(T):	-> S(7) = 9 -> 9 = -t^2 + 7*t -> t^2 - 7t + 9 = 0 -> t1 ~ 5.3 and t2 ~ 1.7. Therefore the 
				values that travel a greater distance are tx for tx > 1.7 and tx < 5.3
				as we are working with integerx it is tx for tx = {2, 3, 4, 5}
	coefficients: A = 1, B = -T, C = S
	t = (-(-T) +- sqrt((-T)^2 - 4 * 1 * S)) / (2 * 1)
*/

#define BUFFER_SIZE 1024
#define MAX_RACES 8

typedef struct {
	long time;
	long distance;
} race_t;

// races are "null-terminated". Race with time == 0 marks the end
race_t race;
long result = 1;

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

	int count = 0;
	double root1, root2;
	double coeff_a = 1;
	double coeff_b = -race.time;
	double coeff_c = race.distance;
	double discriminant = coeff_b * coeff_b - 4 * coeff_a * coeff_c; // b^2 - 4ac
	printf("a = %.2f, b = %.2f, c = %.2f, disc = %.2f --- ", coeff_a, coeff_b, coeff_c, discriminant);

	if (discriminant > 0) {
		root1 = (-coeff_b - sqrt(discriminant)) / (2 * coeff_a);
		root2 = (-coeff_b + sqrt(discriminant)) / (2 * coeff_a);
		printf("Roots %.2f and %.2f\n", root1, root2);
	}

	long start = (root1 != ceil(root1)) ? (long) ceil(root1) : (long) ceil(root1) + 1;
	long end = (root2 != floor(root2)) ? (long) floor(root2) : (long) floor(root2) - 1;
	printf("Values in between: start = %ld, end = %ld (inclusive)\n", start, end);
	for (int i = start; i <= end; i++) {
		count++;
	}

	result *= count;

	printf("Result = %ld\n", result);
	
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
		long count = 0;
		if (strncmp(buffer, "Time:", strlen("Time:")) == 0) {
			for (int i = 0; buffer[i] != '\0'; i++) {
				if (is_digit(buffer[i])) {
					count = (count*10) + (buffer[i] - 0x30); 
				}
			}
			race.time = count;
		} else if (strncmp(buffer, "Distance:", strlen("Distance:")) == 0) {
			for (int i = 0; buffer[i] != '\0'; i++) {
				if (is_digit(buffer[i])) {
					count = (count*10) + (buffer[i] - 0x30); 
				}
			}
			race.distance = count;
		}
	}

	printf("Time: %ld, Distance: %ld\n", race.time, race.distance);

	fclose(file);
	return 1;
}

static int is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

