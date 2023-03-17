#include <iostream>
#include <stdio.h>
#define MAX_COMMAND 100
#define PLAYER_ONE_WON 10
#define PLAYER_TWO_WON -10
#define TIE 0
using namespace std;

struct GameBoard {
	char activePlayer;
	int k;
	int n;
	int m;
	char** board;
};

void SetBoard(GameBoard* board) {
	for (int i = 0; i < board->n; i++)
		for (int j = 0; j < board->m; j++)
			cin >> board->board[i][j];
}

GameBoard* PrepareBoard(int n, int m, int k, char activePlayer) {
	GameBoard* newBoard = new GameBoard;
	newBoard->n = n;
	newBoard->m = m;
	newBoard->k = k;
	newBoard->activePlayer = activePlayer;
	newBoard->board = new char* [newBoard->n];
	for (int i = 0; i < newBoard->n; i++)
		newBoard->board[i] = new char[newBoard->m];
	SetBoard(newBoard);
	return newBoard;
}

void PrintBoard(GameBoard* board) {
	for (int i = 0; i < board->n; i++) {
		for (int j = 0; j < board->m; j++)
			printf("%c ", board->board[i][j]);
		printf("\n");
	}
}

int Evaluate(GameBoard* board) {
	for (int i = 0; i < board->n; i++) {
		for (int j = 0; j < board->m; j++) {
			int count[8] = { 0 };
			for (int l = 0; l < board->k; l++) {
				if (i + l < board->n) {
					if (board->board[i + l][j] == '1')
						count[0]++;
					else if (board->board[i + l][j] == '2')
						count[4]++;
				}
				if (j + l < board->m) {
					if (board->board[i][j + l] == '1')
						count[1]++;
					else if (board->board[i][j + l] == '2')
						count[5]++;
				}
				if (i + l < board->n && j + l < board->m) {
					if (board->board[i + l][j + l] == '1')
						count[2]++;
					else if (board->board[i + l][j + l] == '2')
						count[6]++;
				}
				if (i + l < board->n && j - l >= 0) {
					if (board->board[i + l][j - l] == '1')
						count[3]++;
					else if (board->board[i + l][j - l] == '2')
						count[7]++;
				}
			}
			for (int k = 0; k < 4; k++) {
				if (count[k] == board->k)
					return PLAYER_ONE_WON;
			}
			for (int k = 4; k < 8; k++) {
				if (count[k] == board->k)
					return PLAYER_TWO_WON;
			}
		}
	}
	return 0;
}

void GenerateAllPossibleMovement(GameBoard* board, int option)
{
	int numberOfPossibilities = 0;
	int isOver = Evaluate(board);
	if (isOver == 0)
	{
		for (int i = 0; i < board->n; i++)
		{
			for (int j = 0; j < board->m; j++)
			{
				if (board->board[i][j] == '0')
				{
					if (option == 1)
					{
						board->board[i][j] = board->activePlayer;
						if (Evaluate(board) != 0)
						{
							printf("1 \n");
							PrintBoard(board);
							return;
						}
						board->board[i][j] = '0';
					}
					numberOfPossibilities++;
				}
			}
		}
	}
	printf("%d \n", numberOfPossibilities);
	if (isOver == 0)
	{
		for (int i = 0; i < board->n; i++)
		{
			for (int j = 0; j < board->m; j++)
			{
				if (board->board[i][j] == '0')
				{
					board->board[i][j] = board->activePlayer;
					PrintBoard(board);
					board->board[i][j] = '0';
				}
			}
		}
	}

}

bool IsMovesLeft(GameBoard* board) {
	for (int i = 0; i < board->n; i++) {
		for (int j = 0; j < board->m; j++) {
			if (board->board[i][j] == '0')
				return true;
		}
	}
	return false;
}

int Minimax(GameBoard* board,  bool isMax) {
	int score = Evaluate(board);
	if (score != 0)
		return score;
	if (!IsMovesLeft(board))
		return 0;

	if (isMax) {
		int best = -100;
		for (int i = 0; i < board->n; i++) {
			for (int j = 0; j < board->m; j++) {
				if (board->board[i][j] == '0') {
					board->board[i][j] = '1';
					int result = Minimax(board, !isMax);
					if (result > best)
						best = result;
					board->board[i][j] = '0';
					if (best == PLAYER_ONE_WON)
						return best;
				}
			}
		}
		return best;
	}
	else {
		int best = 100;
		for (int i = 0; i < board->n; i++) {
			for (int j = 0; j < board->m; j++) {
				if (board->board[i][j] == '0') {
					board->board[i][j] = '2';
					int result = Minimax(board, !isMax);
					if (result < best)
						best = result;
					board->board[i][j] = '0';
					if (best == PLAYER_TWO_WON)
						return best;
				}
			}
		}
		return best;
	}
}

void FreeBoard(GameBoard* board) {
	for (int i = 0; i < board->n; i++) 
			free(board->board[i]);
	free(board->board);
}

void SolveGameState(GameBoard* board) {
	int AlreadyOver = Evaluate(board);
	if (AlreadyOver != 0) {
		if (AlreadyOver == PLAYER_ONE_WON)
			printf("FIRST_PLAYER_WINS \n");
		else if (AlreadyOver == PLAYER_TWO_WON)
			printf("SECOND_PLAYER_WINS \n");
		else
			printf("BOTH_PLAYERS_TIE \n");
		return;
	}

	int bestVal;
	if (board->activePlayer == '1') {
		bestVal = -1000;
		for (int i = 0; i < board->n; i++) {
			for (int j = 0; j < board->m; j++) {
				if (board->board[i][j] == '0') {
					board->board[i][j] = board->activePlayer;
					int moveVal = Minimax(board, false);
					board->board[i][j] = '0';
					if (moveVal > bestVal)
						bestVal = moveVal;
					if (bestVal == PLAYER_ONE_WON) {
						printf("FIRST_PLAYER_WINS \n");
						return;
					}
				}
			}
		}
	}
	else {
		bestVal = 1000;
		for (int i = 0; i < board->n; i++) {
			for (int j = 0; j < board->m; j++) {
				if (board->board[i][j] == '0') {
					board->board[i][j] = board->activePlayer;
					int moveVal = Minimax(board, true);
					board->board[i][j] = '0';
					if (moveVal < bestVal)
						bestVal = moveVal;
					if (bestVal == PLAYER_TWO_WON) {
						printf("SECOND_PLAYER_WINS \n");
						return;
					}
				}
			}
		}
	}
	if (bestVal == PLAYER_ONE_WON)
		printf("FIRST_PLAYER_WINS \n");
	else if (bestVal == PLAYER_TWO_WON)
		printf("SECOND_PLAYER_WINS \n");
	else
		printf("BOTH_PLAYERS_TIE \n");
}

int main() {
	int n, m, k;
	char activePlayer;
	char command[MAX_COMMAND];
	while (cin >> command) {
		cin >> n >> m >> k >> activePlayer;
		GameBoard* board = PrepareBoard(n, m, k, activePlayer);

		if (command[0] == 'S') 
			SolveGameState(board);
		else if (command[15] == '_')
			GenerateAllPossibleMovement(board, 1);
		else 
			GenerateAllPossibleMovement(board, 0);

		FreeBoard(board);
		free(board);
	}
	return 0;
}