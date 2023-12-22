#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DIR_COUNT 4
#define MAX_STACK_SIZE 51200
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

static int allowed_directions[ASCII_SIZE]; // Bit manipulation
static int dx[DIR_COUNT] = { 0, 1, 0, -1 };
static int dy[DIR_COUNT] = { -1, 0, 1, 0 };

static void populate_allowed_directions(int*);
static int  dfs(int**, const Vertex, const size_t, const size_t);


typedef struct {
    size_t top;
    size_t capacity; 
    Vertex* elements;
} Stack;

static Stack* new_stack(const size_t); 
static void   free_stack(void*); 
static int    is_empty(const Stack*);
static int    is_full(const Stack*);
static void   push(Stack*, const Vertex);
static Vertex pop(Stack*);


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

    int result = dfs(maze, s_pos, maze_size.row, maze_size.col);

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
    if (!array) { return NULL; }
    for (size_t i = 0; i < rows; i++) {
        array[i] = (int*) malloc(cols * sizeof(int));
        if (!array[i]) { 
            for (size_t j = 0; j < i; j++) {
                free(array[i]);
            } 
            return NULL; 
        }
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

static int dfs(int** maze, const Vertex start, const size_t rows, const size_t cols)
{
    Stack* stack = new_stack(MAX_STACK_SIZE);
    int** visited = new_2Darray(rows, cols); 
    Vertex vertices[MAX_STACK_SIZE] = {0};
    size_t distance = 1;

    push(stack, start);
    visited[start.row][start.col] = distance;

    while (!is_empty(stack)) {
        Vertex current = pop(stack);	
        vertices[distance-1] = current;

        for (size_t dir = 0; dir <= DIR_COUNT; dir++) {
            if ((maze[current.row][current.col] & (1 << dir)) != 0) {   // Bit manipulation
                Vertex next = { 
                            .row = current.row + dy[dir], 
                            .col = current.col + dx[dir]
                };

                size_t opposite_dir = (dir + DIR_COUNT/2) % DIR_COUNT;  // Cycle through directions, DIR_COUNT must be even
                if (next.row < maze_size.row     &&                     // Within bounds
                    next.col < maze_size.col     &&                     // Within bounds
                    !visited[next.row][next.col] &&                     // Not visited
                    (maze[next.row][next.col] & (1 << opposite_dir)) != 0) {    // Next pipe compatible (opposite_dir)

                    push(stack, next);

                    distance++;
                    visited[next.row][next.col] = distance;
                    break;  // For DFS in a circular loop, I just want 1 direction on stack.
                }
            }
        }
    }

    printf("Adjacency matrix\n");
    printf("        ");
    for (size_t i = 0; i < cols; i++) {
        printf("Col %zu\t", i+1);
    }
    printf("\n");
    for (size_t i = 0; i < rows; i++) {
        printf("Row %zu: ", i+1);
        for (size_t j = 0; j < cols; j++) {
            printf("%d\t", visited[i][j]);
        }
        printf("\n");
    }
    printf("Max distance: %zu\n", distance);

    // Shoelace Formula
    int area = 0;
    for (size_t i = 0; i < distance; i++) {
        if (i == distance - 1) {
            // area += (vertices[i].row + vertices[0].row) * (vertices[i].col - vertices[0].col);
            area += (vertices[i].col * vertices[0].row) - (vertices[0].col * vertices[i].row);
        } else {
            // area += (vertices[i].row + vertices[i+1].row) * (vertices[i].col - vertices[i+1].col);
            area += (vertices[i].col * vertices[i+1].row) - (vertices[i+1].col * vertices[i].row);
        }
    }
    area /= 2;
    area = (area >= 0) ? area : -area;
    printf("Area: %d\n", area);

    // Pick's theorem A = i + b/2 - 1
    int inner = area - distance/2 + 1;


    free_2Darray(visited, rows);
    free_stack(stack);
    return inner;
}


static Stack* new_stack(const size_t capacity)
{
    Stack *stack = (Stack*) malloc(sizeof(Stack));
    if (!stack) { return NULL; }
    stack->top= 0;
    stack->capacity = capacity;
    stack->elements = (Vertex*) malloc(capacity * sizeof(Vertex));
    if (!stack->elements) { free(stack); return NULL; }

    return stack;
}

static void free_stack(void* ptr) 
{
    Stack* stack = (Stack*) ptr;
    free(stack->elements);
    free(stack); 
}

static int is_empty(const Stack* stack) 
{ return (stack->top == 0); }

static int is_full(const Stack* stack) 
{ return (stack->top == stack->capacity); }

static void push(Stack* stack, const Vertex value)
{
    if (is_full(stack)) {
        fprintf(stderr, "ERROR: Can't push, stack full %s:%d\n", __FILE__, __LINE__);
    }
    stack->elements[stack->top] = value;
    stack->top++;
    return;
}

static Vertex pop(Stack* stack)
{
    if (is_empty(stack)) {
        fprintf(stderr, "ERROR: Can't pop, stack empty%s:%d\n", __FILE__, __LINE__);
    }
    stack->top--;
    Vertex value = stack->elements[stack->top];
    return value;
}
