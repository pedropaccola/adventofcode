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

static Node nodes[NUM_LETTERS * NUM_LETTERS * NUM_LETTERS] = {0}; // All possible permutations
static char directions[BUFFER_SIZE] = {0};
static size_t index_origin = 0; 
static size_t index_destination = 0;

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
	
	index_origin = name_to_index("AAA");
	index_destination = name_to_index("ZZZ");

	size_t directions_length = strlen(directions);
	int steps = 0;
	// printf("DIRECTIONS: %s\n", directions);
	// printf("DIRECTIONS: Length %zu\n", directions_length);
	// printf("INDEXES: Origin %zu, Dest %zu\n", index_origin, index_destination); 
	// printf("Start:\n");

	for (size_t i = 0; i < directions_length; i = (i+1) % directions_length) { 
		if (index_origin == index_destination) break;

		// printf("Origin: %zu, Direction %c\n", index_origin, directions[i]);
		// print_node(&nodes[index_origin], index_origin);
		if (directions[i] == 'R') {
			index_origin = nodes[index_origin].right;	
		} else {
			index_origin = nodes[index_origin].left;
		}
		steps++;
		// printf("    Next index: %zu, Steps: %d\n", index_origin, steps);
	}

	printf("Result: %d\n", steps);

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

			// print_node(&nodes[index], index);
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
