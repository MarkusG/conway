#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BOARD_SIZE 32
#define IN_RANGE(n) ((n) >= 0 && (n) < BOARD_SIZE ? 1 : 0)

void draw(const int board[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (board[i][j]) {
				case 0:
					printf("\x1b[31m");
					break;
				case 1:
					printf("\x1b[32m");
					break;
				default:
					printf("\x1b[33m");
					break;
			}
			printf("██\x1b[0m");
		}
		putchar('\n');
	}
}

int live_neighbors(const int board[BOARD_SIZE][BOARD_SIZE],
				   const int x, const int y) {
	if (!IN_RANGE(x) || !IN_RANGE(y))
		return -1;
	int result = 0;
	// there's probably a way to reduce the number of checks we do
	if (IN_RANGE(x + 1))
		result += board[x + 1][y];
	if (IN_RANGE(x - 1))
		result += board[x - 1][y];
	if (IN_RANGE(y + 1))
		result += board[x][y + 1];
	if (IN_RANGE(y - 1))
		result += board[x][y - 1];
	if (IN_RANGE(x + 1) && IN_RANGE(y + 1))
		result += board[x + 1][y + 1];
	if (IN_RANGE(x + 1) && IN_RANGE(y - 1))
		result += board[x + 1][y - 1];
	if (IN_RANGE(x - 1) && IN_RANGE(y + 1))
		result += board[x - 1][y + 1];
	if (IN_RANGE(x - 1) && IN_RANGE(y - 1))
		result += board[x - 1][y - 1];

	return result;
}

void step(int board[BOARD_SIZE][BOARD_SIZE]) {
	int new_board[BOARD_SIZE][BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int n = live_neighbors(board, i, j);
			if (board[i][j]) {
				// this cell is alive
				if (n < 2 || n > 3)
					// over/underpopulated; kill this cell
					new_board[i][j] = 0;
				else
					new_board[i][j] = 1;
			} else {
				// this cell is dead
				if (n == 3)
					// 3 live neighbors; make this cell alive
					new_board[i][j] = 1;
				else
					new_board[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = new_board[i][j];
		}
	}
}

int main(void) {
	int board[BOARD_SIZE][BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = 0;
		}
	}
	board[10][10] = 1;
	board[10][11] = 1;
	board[11][10] = 1;
	board[11][11] = 1;

	board[12][12] = 1;
	board[12][13] = 1;
	board[13][12] = 1;
	board[13][13] = 1;
	for (int i = 0; i < 100; i++) {
		printf("\e[1;1H\e[2J");
		printf("Step %d\n", i);
		draw(board);
		step(board);
		usleep(100000);
	}
}
