#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024 
#define TOKENS_SIZE 2048

enum TokenType {
	TOKEN_NUMBER,
	TOKEN_SYMBOL,
	TOKEN_EOF // Terminator
};

typedef struct Token {
	enum TokenType type;
	char* value;
	int row;
	int col;
	int len;
} Token;

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

Token* tokenize(Token* tokens, char* input, int row) {
	int inpLength = strlen(input);
	int col = 0;
	int tokenIndex = 0;
	for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {tokenIndex++;} // Continue replacing the terminator with next token

	while (col < inpLength) {
		if (isDigit(input[col])) {
			tokens[tokenIndex].type = TOKEN_NUMBER;
			int length = 0;
			while (isDigit(input[col])) {
				length++;
				col++;
			}
			tokens[tokenIndex].value = malloc(sizeof(char) * (length + 1));
			strncpy(tokens[tokenIndex].value, &input[col - length], length);
			tokens[tokenIndex].value[length] = '\0';
			tokens[tokenIndex].row = row;
			tokens[tokenIndex].col = col - length;
			tokens[tokenIndex].len = length;
			tokenIndex++;
		} else if (input[col] == '.') {
			col++;
		} else {
			tokens[tokenIndex].type = TOKEN_SYMBOL;
			tokens[tokenIndex].value = malloc(sizeof(char) * 2);
			sprintf(tokens[tokenIndex].value, "%c", input[col]);
			tokens[tokenIndex].row = row;
			tokens[tokenIndex].col = col;
			tokens[tokenIndex].len = 1;
			tokenIndex++;
			col++;
		}
	}
	tokens[tokenIndex].type = TOKEN_EOF; // Terminates
	return tokens;
}

int parse(Token* tokens) {
	int result = 0;
	
	// Continue replacing the terminator with next token
	for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
		if (tokens[i].type != TOKEN_SYMBOL || strcmp(tokens[i].value, "*") != 0) {
			continue;
		}

		int product = 1;
		int adjNumbers = 0;
		int col = tokens[i].col;
		int row = tokens[i].row;

		for (int j = 0; tokens[j].type != TOKEN_EOF; j++) {
			if (tokens[j].type != TOKEN_NUMBER) {
				continue;
			}
			else if (tokens[j].row < row - 1 || tokens[j].row > row + 1) {
				continue;
			}
			else if ((tokens[j].row == row - 1 || tokens[j].row == row + 1) && ((tokens[j].col + tokens[j].len - 1) < col - 1 || tokens[j].col > col + 1)) {
				continue;
			}
			else if (tokens[j].row == row && ((tokens[j].col + tokens[j].len - 1) < col - 1 || tokens[j].col > col + 1)) {
				continue;
			}

			int value = atoi(tokens[j].value);
			product *= value;
			adjNumbers++;

			if (adjNumbers == 2) {
				result += product;
				break;
			}
		}


	}
	return result;
}

int main(int argc, char* argv[]) {
	FILE* fp = fopen(argv[1], "r");
	char fBuffer[BUFFER_SIZE];

	Token* tokens = malloc(sizeof(Token) * TOKENS_SIZE);
	tokens[0].type = TOKEN_EOF; // Terminates
	int row = -1;

	if (fp == NULL) {
		printf("Error: error opening file\n");
		return 1;
	}

	// Tokenize line by line
	while (fgets(fBuffer, sizeof(fBuffer), fp) != NULL) {
		// Remove the newline character if it exists
		int len = strlen(fBuffer);

		if (len > 0 && fBuffer[len - 1] == '\n') {
			fBuffer[len - 1] = '\0';
		}

		row++;

		tokens = tokenize(tokens, fBuffer, row);
	}

	// Printer
	// for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
	// 	char* type;
	// 	switch (tokens[i].type) {
	// 		case 0: type = "TOKEN_NUMBER"; break;
	// 		case 1: type = "TOKEN_SYMBOL"; break;
	// 		default: type = "TOKEN_EOF"; break;
	// 	}
	// 	printf("Token Type: %s, Value: %s, Row: %d, Col: %d, Len: %d\n", type, tokens[i].value, tokens[i].row, tokens[i].col, tokens[i].len);
	// }

	int result = parse(tokens);
	printf("Sum: %d\n", result);

	// Free
	for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
		free(tokens[i].value);
	}
	free(tokens);

	fclose(fp);
	return 0;
}
