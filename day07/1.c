#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

typedef enum {
	TYPE_HIGH = 0,
	TYPE_ONE_PAIR,
	TYPE_TWO_PAIR,
	TYPE_THREE_OF_A_KIND,
	TYPE_FULL_HOUSE,
	TYPE_FOUR_OF_A_KIND,
	TYPE_FIVE_OF_A_KIND,
} scoring_t;

typedef struct {
	int cards[5];
	int bid;
} player_t;

// this array will be 'null-terminated' by a players[i].bid = -1
static player_t players[BUFFER_SIZE];

static int parse_file(const char*);
static int parse_card(char);
static int is_digit(const char);

int main(int argc, char* argv[]) 
{
	int result = 0;

	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (!parse_file(argv[1])) {
		perror("ERROR: fail to parse file");
		return 1;
	}

	// Printer
	for (size_t i = 0; players[i].bid > -1; i++) {
		printf("Player %lu cards: ", i+1);
		for (size_t j = 0; j < 5; j++) {
			printf("%d - ", players[i].cards[j]);
		}
		printf("Player bid = %d\n", players[i].bid);
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
	size_t player = 0;
	while(fgets(buffer, sizeof(buffer), file) != NULL) {
		int bid = 0;
		size_t i = 0;
		while (buffer[i] != ' ') {
			int card = parse_card(buffer[i]);
			if (card != 0) { 
				players[player].cards[i] = card;
			} else { 
				return 0;
			}
			i++;
		}
		i++; // skip whitespace
		while (buffer[i] != '\0' && buffer[i] != '\n') { 
			bid = (bid * 10) + (buffer[i] - '0');
			i++;
		}

		players[player].bid = bid;
		player++;
	}
	// 'null-terminating'
	players[player].bid = -1;

	fclose(file);
	return 1;
}

static int parse_card(char c)
{
	if (is_digit(c)) return c - '0';
	switch (c) {
		case 'T': return 10;
		case 'J': return 11;
		case 'Q': return 12;
		case 'K': return 13;
		case 'A': return 14;
		default: return 0;
	}
}

static int is_digit(const char c) 
{
	// Avoiding the 0;
	return (c > '0' && c <= '9');
}

static scoring_t get_scoring(player_t player) 
{
	int cards[15] = {0};
	for (size_t i = 0; i < 5; i++) {
		cards[player.cards[i]]++;
	}

	return 0;
}
