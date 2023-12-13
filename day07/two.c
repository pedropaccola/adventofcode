#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define HAND_SIZE 5

typedef enum {
	TYPE_HIGH = 0,
	TYPE_ONE_PAIR,
	TYPE_TWO_PAIR,
	TYPE_THREE_OF_A_KIND,
	TYPE_FULL_HOUSE,
	TYPE_FOUR_OF_A_KIND,
	TYPE_FIVE_OF_A_KIND,
} Score;

const char* scoring_names[] = {
	"TYPE_HIGH",
	"TYPE_ONE_PAIR",
	"TYPE_TWO_PAIR",
	"TYPE_THREE_OF_A_KIND",
	"TYPE_FULL_HOUSE",
	"TYPE_FOUR_OF_A_KIND",
	"TYPE_FIVE_OF_A_KIND",
};

typedef struct {
	int cards[HAND_SIZE];
	int bid;
	Score scoring;
} Player;

Player players[BUFFER_SIZE] = {0};
size_t players_size = 0;

static int parse_file(const char*);
static int parse_card(const char);

static void set_scoring(Player*);
static int total_winnings(const Player*, const size_t);
static void print_players(const Player*, const size_t);

static int compare_players(const Player, const Player);
static void swap_players(Player*, Player*);

static size_t sort_partition(Player*, size_t, size_t);
static void sort_players(Player*, size_t, size_t);

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

	for (size_t i = 0; i < players_size; i++) {
		set_scoring(&players[i]);
	}

	sort_players(players, 0, players_size - 1);

	int result = total_winnings(players, players_size);
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
		int bid = 0;
		size_t i = 0;
		while (buffer[i] != ' ') {
			int card = parse_card(buffer[i]);
			if (card != 0) { 
				players[players_size].cards[i] = card;
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

		players[players_size].bid = bid;
		players_size++;
	}

	fclose(file);
	return 1;
}

static int parse_card(const char card)
{
	switch (card) {
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': return (card - '0');
		case 'T': return 10;
		case 'J': return 11;
		case 'Q': return 12;
		case 'K': return 13;
		case 'A': return 14;
		default: return 0;
	}
}

static void set_scoring(Player* player) 
{
	int cards[15] = {0};
	for (size_t i = 0; i < 5; i++) {
		cards[player->cards[i]]++;
	}

	int has_two = 0;
	int has_three = 0;
	for (size_t i = 2; i < 15; i++) {
		if (cards[i] == 2) {
			if (has_two) {
				player->scoring = TYPE_TWO_PAIR;
				return;
			}
			has_two = 1;
		} else if (cards[i] == 3) {
			has_three = 1;
		} else if (cards[i] == 4) {
			player->scoring = TYPE_FOUR_OF_A_KIND;
			return;
		} else if (cards[i] == 5) {
			player->scoring = TYPE_FIVE_OF_A_KIND;
			return;
		}
	}

	if (has_three) {
		if (has_two) {
			player->scoring = TYPE_FULL_HOUSE;
		} else {
			player->scoring = TYPE_THREE_OF_A_KIND;
		}
		return;
	}

	if (has_two) {
		player->scoring = TYPE_ONE_PAIR;
		return;
	}

	player->scoring = TYPE_HIGH;
}

static void print_players(const Player* players, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		printf("Player %zu cards: ", i+1);
		for (size_t j = 0; j < HAND_SIZE; j++) {
			printf("%d - ", players[i].cards[j]);
		}
		printf("Player bid = %d - ", players[i].bid);
		printf("Player scoring = %s\n", scoring_names[players[i].scoring]);
	}
}

static size_t sort_partition(Player* players, size_t low, size_t high)
{
	size_t greatest_idx = low - 1;

	for (size_t i = low; i < high; i++) {
		if (!compare_players(players[i], players[high])) {
			greatest_idx++;
			swap_players(&players[i], &players[greatest_idx]);
		}
	}
	
	swap_players(&players[greatest_idx+1], &players[high]);

	return greatest_idx + 1;
}
static void sort_players(Player* players, size_t low, size_t high)
{
	if (low < high) {
		size_t pivot = sort_partition(players, low, high);

		sort_players(players, low, pivot - 1);
		sort_players(players, pivot + 1, high);
	}

}

static int compare_players(const Player player_1, const Player player_2) 
{
	/* 
	 * return 1 if player 1 > player 2
	 * return 0 otherwise
	 * supposing two tied players won't have the same cards in the same order
	*/ 
	if (player_1.scoring != player_2.scoring) {
		return player_1.scoring > player_2.scoring;
	}

	size_t i = 0;
	while (player_1.cards[i] == player_2.cards[i] && i < HAND_SIZE) {
		i++;
	}
	return player_1.cards[i] > player_2.cards[i];
}
static void swap_players(Player* player_1, Player* player_2)
{
	Player temp = *player_1;
	*player_1 = *player_2;
	*player_2 = temp;
}

static int total_winnings(const Player* players, const size_t size)
{
	int result = 0; 
	for (size_t i = 0; i < size; i++) {
		result += players[i].bid * (i+1);
	}
      return result;
}

