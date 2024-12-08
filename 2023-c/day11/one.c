#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define GALAXIES_CAP 1024


typedef struct {
    size_t x, y;
} Vertex;

Vertex galaxies[GALAXIES_CAP] = {0};
size_t galaxies_size = 0;

static int parse_file(const char*);


int main(int argc, char* argv[]) 
{
    if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (!parse_file(argv[1])) {
        perror("ERROR: fail to parse file");
        return 1;
    }

    int result = 0;

    for (size_t i = 0; i < galaxies_size; i++) {
        printf("Galaxy %zu, x = %zu, y = %zu\n", i, galaxies[i].x, galaxies[i].y);
    }

    printf("Result: %d\n", result);

    return 0;
}

static int parse_file(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "ERROR: can't open file %s\n", filename);
        return 0;
    }
    
    int row = 0;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int col = 0;
        while (buffer[col] != '\n' && buffer[col] != '\0') {
            if (buffer[col] == '#') {
                Vertex node = {
                    .y = col,
                    .x = row,
                };
                galaxies[galaxies_size] = node;
                galaxies_size++;
            }
            col++;
        }
        row++;
    }

    fclose(file);
    return 1;
}
