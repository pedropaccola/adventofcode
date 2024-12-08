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
	int time;
	int distance;
} race_t;

// races are "null-terminated". Race with time == 0 marks the end
race_t races[MAX_RACES];
int result = 1;

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

	for (int i = 0; races[i].time != 0; i++) {
		int count = 0;
		double root1, root2;
		double coeff_a = 1;
		double coeff_b = -races[i].time;
		double coeff_c = races[i].distance;
		double discriminant = coeff_b * coeff_b - 4 * coeff_a * coeff_c; // b^2 - 4ac
		printf("a = %.2f, b = %.2f, c = %.2f, disc = %.2f --- ", coeff_a, coeff_b, coeff_c, discriminant);

		if (discriminant > 0) {
			root1 = (-coeff_b - sqrt(discriminant)) / (2 * coeff_a);
			root2 = (-coeff_b + sqrt(discriminant)) / (2 * coeff_a);
			printf("Roots %.2f and %.2f\n", root1, root2);
		}

		int start = (root1 != ceil(root1)) ? (int) ceil(root1) : (int) ceil(root1) + 1;
		int end = (root2 != floor(root2)) ? (int) floor(root2) : (int) floor(root2) - 1;
		printf("Values in between: start = %d, end = %d (inclusive)\n", start, end);
		for (int i = start; i <= end; i++) {
			count++;
		}

		result *= count;
	}

	printf("Result = %d\n", result);
	
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
		int count = 0;
		int n = 0;
		int offset = 0;
		if (strncmp(buffer, "Time:", strlen("Time:")) == 0) {
			memset(buffer, ' ', strlen("Time:"));
			while (sscanf(buffer + offset, " %d%n", &races[count].time, &n) == 1) {
				count++;
				offset += n;
			}
			races[count].time = 0;
		} else if (strncmp(buffer, "Distance:", strlen("Distance:")) == 0) {
			memset(buffer, ' ', strlen("Distance:"));
			while (sscanf(buffer + offset, " %d%n", &races[count].distance, &n) == 1) {
				count++;
				offset += n;
			}
			races[count].distance = 0;
		}
	}

	for (int i = 0; races[i].time != 0; i++) {
		printf("Time: %d, Distance: %d\n", races[i].time, races[i].distance);
	}

	fclose(file);
	return 1;
}
