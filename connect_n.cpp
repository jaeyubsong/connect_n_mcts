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
    private:
        int moves;
        char** board;
        int whichPlayerWin;
    
    public:
        int getMoves() {
            return moves;
        }

        char** getBoard() {
            return board;
        }

        int getWhichPlayerWin() {
            return whichPlayerWin;
        }

        ConnectNBoard(int flag) : moves(0), whichPlayerWin(0) {
            board = (char**) malloc(BOARD_SIZE * sizeof(char *));
            for (int i = 0; i < BOARD_SIZE; i++) {
                board[i] = (char *) malloc(BOARD_SIZE * sizeof(char));
            }
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

        bool isWin(int player) {
            char playerMove;
            if (player == PLAYER1) {
                playerMove = PLAYER1MOVE;
            } else if (player == PLAYER2) {
                playerMove = PLAYER2MOVE;
            } else {
                printf("Wrong player\n");
                return -1;
            }
            // Check horizontal
            int consec = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == playerMove) {
                        consec++;
                        if (consec == WIN_LENGTH) {
                            printf("%c: Win by horizontal\n", playerMove);
                            whichPlayerWin = player;
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
                    if (board[j][i] == playerMove) {
                        consec++;
                        if (consec == WIN_LENGTH) {
                            printf("%c: Win by vertical\n", playerMove);
                            whichPlayerWin = player;
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
                        if (board[i+k][j+k] == playerMove) {
                            consec++;
                            if (consec == WIN_LENGTH) {
                                printf("%c: Win by right diagonal\n", playerMove);
                                whichPlayerWin = player;
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
                        if (board[i-k][j-k] == playerMove) {
                            consec++;
                            if (consec == WIN_LENGTH) {
                                printf("%c: Win by left diagonal\n", playerMove);
                                whichPlayerWin = player;
                                return true;
                            }
                        }
                        else
                            consec = 0;
                    }
                    consec = 0;
                }
            }

            printf("%c: Not win\n", playerMove);
            return false;
        }

        int makeMove(int i, int j, int player) {
            char playerMove;
            int opponent;
            if (player == PLAYER1) {
                playerMove = PLAYER1MOVE;
                opponent = PLAYER2;
            } else if (player == PLAYER2) {
                playerMove = PLAYER2MOVE;
                opponent = PLAYER1;
            } else {
                printf("Wrong player\n");
                return -1;
            }

            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                printf("The board is full\n");
                whichPlayerWin = opponent;
                return -1;
            }
            if (i < 0 || i >= BOARD_SIZE
            || j < 0 || j >= BOARD_SIZE) {
                printf("Invalid move of i: %d, j: %d in makeMove\n", i, j);
                whichPlayerWin = opponent;
                return -1;
            }
            if (board[i][j] == ' ') {
                board[i][j] = playerMove;
                moves++;
                return (i*BOARD_SIZE + j);
            } else {
                printf("Stone exists at i: %d, j: %d in makeMove\n", i, j);
                whichPlayerWin = opponent;
                return -1;
            }

        }

        int makeMove(int move, int player) {
            char playerMove;
            int opponent;
            if (player == PLAYER1) {
                playerMove = PLAYER1MOVE;
                opponent = PLAYER2;
            } else if (player == PLAYER2) {
                playerMove = PLAYER2MOVE;
                opponent = PLAYER1;
            } else {
                printf("Wrong player\n");
                return -1;
            }

            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                printf("The board is full\n");
                whichPlayerWin = opponent;
                return -1;
            }
            if (move < 0 || move >= BOARD_SIZE * BOARD_SIZE) {
                printf("Invalid move of %d in makeMove\n", move);
                whichPlayerWin = opponent;
                return -1;
            }
            int i = move / BOARD_SIZE;
            int j = move % BOARD_SIZE;
            if (board[i][j] == ' ') {
                board[i][j] = playerMove;
                moves++;
                return (i*BOARD_SIZE + j);
            } else {
                printf("Stone exists at i: %d, j: %d in makeMove\n", i, j);
                whichPlayerWin = opponent;
                return -1;
            }
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

        int playGame(int (*player1Func)(char**, int), int (*player2Func)(char**, int), bool showBoard) {
            bool gameDone = false;
            int whoseTurn = PLAYER1;
            int myMove;
            while (true) {
                if (moves >= BOARD_SIZE * BOARD_SIZE) {
                    break;
                }
                if (whoseTurn == PLAYER1) {
                    myMove = (player1Func)(board, PLAYER1);
                    myMove = this->makeMove(myMove, PLAYER1);
                    gameDone = this->isWin(PLAYER1);
                    if (showBoard) {
                        this->showBoard();
                    }
                    if (gameDone)
                        break;
                    whoseTurn = PLAYER2;
                }
                else if (whoseTurn == PLAYER2) {
                    myMove = (player2Func)(board, PLAYER2);
                    myMove = this->makeMove(myMove, PLAYER2);
                    gameDone = this->isWin(PLAYER2);
                    if (showBoard) {
                        this->showBoard();
                    }
                    if (gameDone)
                        break;
                    whoseTurn = PLAYER1;
                }
            }
        }

        int finishGame(bool printWinner) {
            if (printWinner) {
                if (whichPlayerWin == PLAYER1)
                    printf("Player1 won\n");
                else if (whichPlayerWin == PLAYER2)
                    printf("Player2 won\n");
                else
                    printf("No one won\n");
            }
            free(board);
            return whichPlayerWin;
        }
};

void printMove(int myMove) {
    char startChar = 'A';
    int startInt = BOARD_SIZE;
    startChar += myMove % BOARD_SIZE;
    startInt -= myMove / BOARD_SIZE;
    printf("%c%d", startChar, startInt);
}

int charToMove(char myChar, int myInt) {
    int offset = myChar - 'A';
    return (BOARD_SIZE - myInt) * BOARD_SIZE + offset;
}

bool checkValidInput (char myChar, int myInt) {
    int offset = myChar - 'A';
    if (offset < 0 || offset > BOARD_SIZE
    || myInt < 0 || myInt > BOARD_SIZE) {
        return false;
    }
    return true;
}

int getHumanMove(char** board, int player) {
    char myChar;
    int myInt;
    int myMove;
    int i,j;
    while (true) {
        if (scanf("%c%d", &myChar, &myInt) == 2) {
            if (checkValidInput(myChar, myInt) == false) {
                printf("Invalid move, try again\n");
                continue;
            }
            myMove = charToMove(myChar, myInt);
            i = myMove / BOARD_SIZE;
            j = myMove % BOARD_SIZE;
            if (board[i][j] != ' ') {
                printf("Invalid move, try again\n");
                continue;
            }
            return myMove;
        }
    }
}


int main() {
    assert(BOARD_SIZE >= WIN_LENGTH);
    ConnectNBoard board(EMPTY_INIT);
    board.showBoard();
    board.playGame(getHumanMove, getHumanMove, true);

    board.finishGame(true);
}