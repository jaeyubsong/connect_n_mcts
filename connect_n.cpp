#include <cstdio>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <assert.h>
#include <vector>
#include <algorithm>
using namespace std;

#define PLAYER1 1
#define PLAYER2 2

#define BOARD_SIZE 5 // Size of board
#define WIN_LENGTH 3 // Consecutive stones to win

// Computer will move 'O'
// Player will move 'X'
#define PLAYER1MOVE 'O'
#define PLAYER2MOVE 'X'

#define EMPTY_INIT 0
#define RANDOM_INIT 1

default_random_engine dre (chrono::steady_clock::now().time_since_epoch().count());     // provide seed
int random (int lim)
{
    uniform_int_distribution<int> uid {0,lim};   // help dre to generate nos from 0 to lim (lim included);
    return uid(dre);    // pass dre as an argument to uid to generate the random no
}

class ConnectNBoard {
    int moves;
    char board[BOARD_SIZE][BOARD_SIZE];
    
    public:
        ConnectNBoard(int flag) : moves(0) {
            if (flag == EMPTY_INIT) {
                for (int i = 0; i < BOARD_SIZE; i++) {
                    for (int j = 0; j < BOARD_SIZE; j++) {
                        board[i][j] = ' ';
                    }
                }
            }
            else if (flag == RANDOM_INIT) {
                char moves[3] = {' ', PLAYER1MOVE, PLAYER2MOVE};
                for (int i = 0; i < BOARD_SIZE; i++) {
                    for (int j = 0; j < BOARD_SIZE; j++) {
                        board[i][j] = moves[random(2)];
                        if (board[i][j] != ' ') {
                            this->moves++;
                        }
                    }
                }    
            }
        }
        void showBoard() {
            printf("Board is showed below\n");
            char startChar = 'A';
            int startNum = BOARD_SIZE;
            printf("   ");
            for (int i = 0; i < BOARD_SIZE; i++) {
                printf(" %c  ", startChar++);
            }
            printf("\n");
            for (int i = 0; i < BOARD_SIZE; i++) {
                if (startNum < 10) {
                    printf(" ");
                }
                printf("%d ", startNum--);
                for (int j = 0; j < BOARD_SIZE; j++) {
                    printf(" %c ", board[i][j]);
                    if (j != BOARD_SIZE-1) {
                        printf("|");
                    }
                }
                printf("\n   ");
                for (int k = 0; k < BOARD_SIZE; k++) {
                    printf("----");
                }
                printf("\n");
            }
        }
        bool isWin(char player) {
            // Check horizontal
            int consec = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == player) {
                        consec++;
                        if (consec == WIN_LENGTH) {
                            printf("%c: Win by horizontal\n", player);
                            return true;
                        }
                    } else
                        consec = 0;
                }
                consec = 0;
            }

            // Check vertical
            consec = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[j][i] == player) {
                        consec++;
                        if (consec == WIN_LENGTH) {
                            printf("%c: Win by vertical\n", player);
                            return true;
                        }
                    } else
                        consec = 0;
                }
                consec = 0;
            }

            // Check right diagonal
            consec = 0;
            int max_inspect;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    max_inspect = BOARD_SIZE - max(i,j);
                    for (int k = 0; k < max_inspect; k++) {
                        if (board[i+k][j+k] == player) {
                            consec++;
                            if (consec == WIN_LENGTH) {
                                printf("%c: Win by right diagonal\n", player);
                                return true;
                            }
                        }
                        else
                            consec = 0;
                    }
                    consec = 0;
                }
            }

            // Check left diagonal
            consec = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    max_inspect = BOARD_SIZE - max(BOARD_SIZE - i,j);
                    for (int k = 0; k < max_inspect; k++) {
                        if (board[i-k][j-k] == player) {
                            consec++;
                            if (consec == WIN_LENGTH) {
                                printf("%c: Win by left diagonal\n", player);
                                return true;
                            }
                        }
                        else
                            consec = 0;
                    }
                    consec = 0;
                }
            }

            printf("%c: Not win\n", player);
            return false;
        }

        int makeMove(int i, int j, char playerMove) {
            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                return -1;
            }
            if (board[i][j] == ' ') {
                board[i][j] = playerMove;
                moves++;
                return (i*BOARD_SIZE + j);
            }
            return -1;
        }

        int makeRandomMove(char playerMove) {
            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                return -1;
            }
            int i,j;
            i = random(BOARD_SIZE-1);
            j = random(BOARD_SIZE-1);
            while (board[i][j] != ' ') {
                i = random(BOARD_SIZE-1);
                j = random(BOARD_SIZE-1);
            }
            board[i][j] = playerMove;
            moves++;
            return i*BOARD_SIZE + j;
        }

        int getMoves() {
            return moves;
        }
};

void printMove(int myMove) {
    char startChar = 'A';
    int startInt = BOARD_SIZE;
    startChar += myMove % BOARD_SIZE;
    startInt -= myMove / BOARD_SIZE;
    printf("%c%d", startChar, startInt);
}


int main() {
    assert(BOARD_SIZE >= WIN_LENGTH);
    ConnectNBoard board(RANDOM_INIT);
    int myMove = board.makeRandomMove(PLAYER1MOVE);
    printMove(myMove);
    printf("\n");
    myMove = board.makeRandomMove(PLAYER2MOVE);
    printMove(myMove);
    printf("\n");
    bool player1Win = board.isWin(PLAYER1MOVE);
    bool player2Win = board.isWin(PLAYER2MOVE);
    board.showBoard();
    if (player1Win)
        printf("Player1 won\n");
    else if (player2Win)
        printf("Player2 won\n");
    else
        printf("No one won\n");
    printf("Moves: %d\n", board.getMoves());
    
}