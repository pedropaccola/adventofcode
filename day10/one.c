#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DIR_COUNT 4
#define MAX_QUEUE_SIZE 1024
#define BUFFER_SIZE 1024
#define ASCII_SIZE 128


static int parse_file(const char*);


typedef struct {
	size_t row, col;
} Vertex;

static int**  maze;
static Vertex maze_size = { .row = 0, .col = 0 };
static Vertex s_pos = { .row = 0, .col = 0 };

static int** new_2Darray(const size_t, const size_t);
static void  free_2Darray(int**, const size_t);


typedef enum {
	DIR_NORTH = 0, //1 << NORTH = 0b0001 == 1
	DIR_EAST,      //1 << EAST  = 0b0010 == 2
	DIR_SOUTH,     //1 << SOUTH = 0b0100 == 4
	DIR_WEST,      //1 << WEST  = 0b1000 == 8
} Directions;

static int allowed_directions[ASCII_SIZE];
static int dx[DIR_COUNT] = { 0, 1, 0, -1 };
static int dy[DIR_COUNT] = { -1, 0, 1, 0 };

static void populate_allowed_directions(int*);
static int  bfs(int**, const Vertex, const size_t, const size_t);
static int  is_valid_pos(const int, const int, const int, const int);


typedef struct {
	size_t front, rear;
	size_t size, capacity;
	Vertex* elements;
} Queue;

static Queue* new_queue(const size_t); 
static void   free_queue(void*); 
static int    is_empty(const Queue*);
static int    is_full(const Queue*);
static void   enqueue(Queue*, const Vertex);
static Vertex dequeue(Queue*);


int main(int argc, char* argv[]) 
{
	populate_allowed_directions(allowed_directions);

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (!parse_file(argv[1])) {
		perror("ERROR: fail to parse file");
		return 1;
	}

	
	printf("Rows: %zu, Cols: %zu\n", maze_size.row, maze_size.col);
	printf("S-Row: %zu, S-Col: %zu\n", s_pos.row+1, s_pos.col+1);
	for (size_t i = 0; i < maze_size.row; i++) {
		printf("Row %zu: ", i+1);
		for (size_t j = 0; j < maze_size.col; j++) {
			printf("%d\t", maze[i][j]);
		}
		printf("\n");
	}

	int result = bfs(maze, s_pos, maze_size.row, maze_size.col);

	printf("Result: %d\n", result);

	free_2Darray(maze, maze_size.row);
	return 0;
}

static int parse_file(const char* filename) 
{
	FILE* file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "ERROR: can't open file %s\n", filename);
		return 0;
	}
	
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
		printf("\n");
		// Just one pass 
		if (maze_size.row == 0) {
			int length = strlen(buffer);
			maze_size.col = (buffer[length-1] == '\n') ? length-1 : length;
		}
		maze_size.row++;
	}


	maze = new_2Darray(maze_size.row, maze_size.col);

	rewind(file);
	maze_size.row = 0;

	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		size_t i = 0;
		while (buffer[i] != '\0') {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				break;
			}
			if (buffer[i] == 'S') {
				s_pos.row = maze_size.row;
				s_pos.col = i;
			}
			maze[maze_size.row][i] = allowed_directions[(int) buffer[i]];
			i++;
		}
		maze_size.row++;
	}

	fclose(file);
	return 1;
}

static void populate_allowed_directions(int* allowed_directions)
{
	for (size_t char_code = 0; char_code < ASCII_SIZE; char_code++) {
		allowed_directions[char_code] = 0;
		switch (char_code) {
			case '-': 
				allowed_directions[char_code] |= (1 << DIR_EAST) | (1 << DIR_WEST);
				printf("'-' is %d\n", allowed_directions[char_code]);
				break;
			case '|': 
				allowed_directions[char_code] |= (1 << DIR_NORTH) | (1 << DIR_SOUTH);
				printf("'|' is %d\n", allowed_directions[char_code]);
				break;
			case 'L': 
				allowed_directions[char_code] |= (1 << DIR_NORTH) | (1 << DIR_EAST);
				printf("'L' is %d\n", allowed_directions[char_code]);
				break;
			case 'J': 
				allowed_directions[char_code] |= (1 << DIR_NORTH) | (1 << DIR_WEST);
				printf("'J' is %d\n", allowed_directions[char_code]);
				break;
			case 'F': 
				allowed_directions[char_code] |= (1 << DIR_SOUTH) | (1 << DIR_EAST);
				printf("'F' is %d\n", allowed_directions[char_code]);
				break;
			case '7': 
				allowed_directions[char_code] |= (1 << DIR_SOUTH) | (1 << DIR_WEST);
				printf("'7' is %d\n", allowed_directions[char_code]);
				break;
			case 'S':
				allowed_directions[char_code] |= (1 << DIR_NORTH) | (1 << DIR_SOUTH) | (1 << DIR_EAST) | (1 << DIR_WEST);
				printf("'S' is %d\n", allowed_directions[char_code]);
				break;
		}
	}
}

static int** new_2Darray(size_t rows, size_t cols)
{
	int** array = (int **) malloc(rows * sizeof(int*));
	for (size_t i = 0; i < rows; i++) {
		array[i] = (int*) malloc(cols * sizeof(int));
		memset(array[i], 0, cols * sizeof(int));
	}
	return array;
}

static void free_2Darray(int** array, size_t rows)
{
	for (size_t i = 0; i < rows; i++) {
		free(array[i]);
	}
	free(array);
}

static int bfs(int** maze, const Vertex start, const size_t rows, const size_t cols)
{
	Queue* queue = new_queue(MAX_QUEUE_SIZE);
	int** visited = new_2Darray(rows, cols); 
	int max_value = 0;

	enqueue(queue, start);
	visited[start.row][start.col]++;

	while (!is_empty(queue)) {
		Vertex current = dequeue(queue);	

		for (size_t dir = 0; dir <= DIR_COUNT; dir++) {
			if ((maze[current.row][current.col] & (1 << dir)) != 0) {
				Vertex next = { 
					.row = current.row + dy[dir], 
					.col = current.col + dx[dir] };
				int opposite_dir = (dir + DIR_COUNT/2) % 4;
				if (is_valid_pos(next.row, next.col, maze_size.row, maze_size.col) &&
						 !visited[next.row][next.col] &&
					 	 (maze[next.row][next.col] & (1 << opposite_dir)) != 0) {
					enqueue(queue, next);
					int sum = visited[current.row][current.col] + 1;
					visited[next.row][next.col] = sum;
					max_value = (sum > max_value) ? sum : max_value;
				}
			}
		}
	}

	printf("Adjacency matrix\n");
	for (size_t i = 0; i < rows; i++) {
		printf("Row %zu: ", i+1);
		for (size_t j = 0; j < cols; j++) {
			printf("%d\t", visited[i][j]);
		}
		printf("\n");
	}

	free_2Darray(visited, rows);
	free_queue(queue);
	return max_value - 1;   // Because started at 1, but should have started at 0;
}

static int is_valid_pos(const int row, const int col, const int rows, const int cols)
{
	return (row < rows && row >= 0 &&
		col < cols && col >= 0);
}

static Queue* new_queue(const size_t capacity)
{
	Queue *queue = (Queue*) malloc(sizeof(Queue));
	if (!queue) { return NULL; }
	queue->front = 0;
	queue->rear = 0;
	queue->size = 0;
	queue->capacity = capacity;
	queue->elements = (Vertex*) malloc(capacity * sizeof(Vertex));
	if (!queue->elements) { free(queue); return NULL; }

	return queue;
}

static void free_queue(void* ptr) 
{
	Queue* queue = (Queue*) ptr;
	free(queue->elements);
	free(queue); 
}

static int is_empty(const Queue* queue) { return (queue->size == 0); }

static int is_full(const Queue* queue) { return (queue->size == queue->capacity); }

static void enqueue(Queue* queue, const Vertex value)
{
	if (is_full(queue)) {
		fprintf(stderr, "ERROR: Can't enqueue, queue full %s:%d\n", __FILE__, __LINE__);
	}
	queue->elements[queue->rear] = value;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->size++;
	return;
}

static Vertex dequeue(Queue* queue)
{
	if (is_empty(queue)) {
		fprintf(stderr, "ERROR: Can't dequeue, queue empty%s:%d\n", __FILE__, __LINE__);
	}
	Vertex value = queue->elements[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size--;
	return value;
}
