#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 8
#define EMPTY '.'
#define PLAYER1 'X'
#define PLAYER2 'O'

void initBoard(char* boardData);
void drawBoard();
int isValidMove(int row, int col, char player);
void makeMove(int row, int col, char player);
int countPieces(char player);
int isGameOver();
int main_game();

#endif /* GAME_H */
