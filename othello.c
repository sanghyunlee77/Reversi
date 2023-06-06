#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define BOARD_SIZE 8
#define EMPTY '.'
#define PLAYER1 'X'
#define PLAYER2 'O'

char board[BOARD_SIZE][BOARD_SIZE];
int selectedRow = -1;
int selectedCol = -1;

void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }

    // Initial pieces
    board[3][3] = PLAYER1;
    board[4][4] = PLAYER1;
    board[3][4] = PLAYER2;
    board[4][3] = PLAYER2;
}

void drawBoard() {
    clear();

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Draw the board grid
            attron(COLOR_PAIR(1));
            mvprintw(i * 2, j * 4, "+---");
            mvprintw(i * 2 + 1, j * 4, "| %c ", board[i][j]);
        }
        mvprintw(i * 2, BOARD_SIZE * 4, "+");
        mvprintw(i * 2 + 1, BOARD_SIZE * 4, "|");
    }

    // Draw the bottom border
    for (int j = 0; j < BOARD_SIZE; j++) {
        attron(COLOR_PAIR(1));
        mvprintw(BOARD_SIZE * 2, j * 4, "+---");
    }
    mvprintw(BOARD_SIZE * 2, BOARD_SIZE * 4, "+");

    // Highlight the selected cell
    if (selectedRow != -1 && selectedCol != -1) {
        attron(COLOR_PAIR(2));
        mvprintw(selectedRow * 2, selectedCol * 4, "+---");
        mvprintw(selectedRow * 2 + 1, selectedCol * 4, "| %c ", board[selectedRow][selectedCol]);
    }

    refresh();
}

int isValidMove(int row, int col, char player) {
    if (board[row][col] != EMPTY) {
        return 0;
    }

    int dr, dc;
    char opponent = (player == PLAYER1) ? PLAYER2 : PLAYER1;

    // Check all 8 directions
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }

            int r = row + dr;
            int c = col + dc;
            int foundOpponent = 0;

            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (board[r][c] == EMPTY) {
                    break;
                }

                if (board[r][c] == opponent) {
                    foundOpponent = 1;
                } else if (board[r][c] == player && foundOpponent) {
                    return 1;
                } else {
                    break;
                }

                r += dr;
                c += dc;
            }
        }
    }

    return 0;
}

void makeMove(int row, int col, char player) {
    int dr, dc;
    char opponent = (player == PLAYER1) ? PLAYER2 : PLAYER1;

    board[row][col] = player;

    // Flip opponent pieces in all valid directions
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }

            int r = row + dr;
            int c = col + dc;
            int foundOpponent = 0;

            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (board[r][c] == EMPTY) {
                    break;
                }

                if (board[r][c] == opponent) {
                    foundOpponent = 1;
                } else if (board[r][c] == player && foundOpponent) {
                    while (r != row || c != col) {
                        board[r][c] = player;
                        r -= dr;
                        c -= dc;
                    }

                    break;
                } else {
                    break;
                }

                r += dr;
                c += dc;
            }
        }
    }
}

int countPieces(char player) {
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                count++;
            }
        }
    }

    return count;
}

int isGameOver() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (isValidMove(i, j, PLAYER1) || isValidMove(i, j, PLAYER2)) {
                return 0;
            }
        }
    }

    return 1;
}

int main() {
    initscr(); // Initialize ncurses
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Grid color
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Selected cell color

    initBoard();

    char currentPlayer = PLAYER1;

    while (!isGameOver()) {
        drawBoard();

        mvprintw(BOARD_SIZE * 2 + 2, 0, "Player %c's turn. Click on a cell to make a move.", currentPlayer);
        refresh();

        MEVENT event;
        int ch = getch();
        if (ch == KEY_MOUSE && getmouse(&event) == OK) {
            int row = event.y / 2;
            int col = event.x / 4;

            if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && isValidMove(row, col, currentPlayer)) {
                makeMove(row, col, currentPlayer);
                currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
            }
        }
    }

    drawBoard();

    int player1Count = countPieces(PLAYER1);
    int player2Count = countPieces(PLAYER2);

    mvprintw(BOARD_SIZE * 2 + 4, 0, "Game Over");
    mvprintw(BOARD_SIZE * 2 + 5, 0, "Player X: %d", player1Count);
    mvprintw(BOARD_SIZE * 2 + 6, 0, "Player O: %d", player2Count);
    mvprintw(BOARD_SIZE * 2 + 7, 0, "Winner: %c", (player1Count > player2Count) ? PLAYER1 : PLAYER2);

    refresh();

    getch(); // Wait for user input

    endwin(); // End ncurses

    return 0;
}
// dsfad
