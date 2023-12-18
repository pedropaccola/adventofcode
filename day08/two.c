#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define NODE_NAME_SIZE 3
#define NUM_LETTERS 26 //('Z' - 'A' + 1)

typedef struct {
	char name[NODE_NAME_SIZE + 1];
	int left, right;
} Node;

static size_t name_to_index(const char*);
static void print_node(const Node*, const int);
static int parse_file(const char*);
static long gcd(const long, const long);
static long lcm(const long, const long*);

static Node nodes[NUM_LETTERS * NUM_LETTERS * NUM_LETTERS] = {0};    // Size for all possible permutations; doesn't require a count;
static Node* a_nodes[NUM_LETTERS * NUM_LETTERS * NUM_LETTERS] = {0}; // Nodes ending with 'A'. Just pointers to the array above;
static long results[NUM_LETTERS * NUM_LETTERS * NUM_LETTERS] = {0};   // Result for each a_node. It appears to be cyclical
static size_t a_nodes_count = 0;				     // Size for both a_nodes and results
static char directions[BUFFER_SIZE] = {0};

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
	
	size_t directions_length = strlen(directions);

	for (size_t j = 0; j < a_nodes_count; j++) { 
		printf("Checking node %s\n", a_nodes[j]->name);
		long steps = 0;
		for (size_t i = 0; i < directions_length; i = (i+1) % directions_length) { 
			if (directions[i] == 'R') {
				a_nodes[j] = &nodes[a_nodes[j]->right];
			} else {
				a_nodes[j] = &nodes[a_nodes[j]->left];
			}
			steps++;

			if (a_nodes[j]->name[2] == 'Z'){
				printf("    Reached Node %s, steps: %ld\n", a_nodes[j]->name, steps);
				results[j] = steps;
				break;
			}
		}
	}

	printf("Results:\n");
	for (size_t i = 0; i < a_nodes_count; i++) {
		printf("For element %zu, Steps: %ld\n", i, results[i]);
	}

	long result = lcm(a_nodes_count, results);
	printf("LCM = %ld\n", result);

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
		if (
			buffer[0] == '\n'
		) { 
			continue;
		} else if (buffer[4] == '=') {
			int index = name_to_index(&buffer[0]);

			nodes[index].left = name_to_index(&buffer[7]);
			nodes[index].right= name_to_index(&buffer[12]);
			strncpy(nodes[index].name, &buffer[0], NODE_NAME_SIZE);

			if (nodes[index].name[2] == 'A') {
				a_nodes[a_nodes_count] = &nodes[index];
				if (a_nodes[a_nodes_count] == NULL) {printf("Failed\n");}
				print_node(a_nodes[a_nodes_count], a_nodes_count);
				a_nodes_count++;
			}

		} else {
			strcpy(directions, buffer);
			char* newline = strchr(directions, '\n');
			if (newline != NULL) {
				*newline = '\0';
			}
		}
	}

	fclose(file);
	return 1;
}

static size_t name_to_index(const char* name)
{
	size_t output = 0;
	for (size_t i = 0; i < NODE_NAME_SIZE; i++) {
		// AAA = 0, AAB = 1 ... AAZ = 25, ABA = 26 ... ZZZ = (25*26*26 + 25*26 + 25)
		output = (output * NUM_LETTERS) + (name[i] - 'A'); 
	}
	return output;
}

static void print_node(const Node* node, const int index)
{
	printf("Node: %s, Index: %d, Left: %d, Right: %d\n",
		node->name, index, node->left, node->right);
}

static long gcd(const long a, const long b)
{
	if (b == 0) return a;
	else return gcd(b, a%b);
}

static long lcm(const long num_args, const long* args)
{
	if (num_args == 1) return args[0];
	return (args[0] * lcm(num_args - 1, args + 1)) / gcd(args[0], lcm(num_args - 1, args + 1));
}
