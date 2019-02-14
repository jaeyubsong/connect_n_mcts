#include <cstdio>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <assert.h>
#include <vector>
#include <algorithm>
using namespace std;

#define EMPTY 0
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

int random(int lim);
void printMove(int myMove);
int charToMove(char myChar, int myInt);
bool checkValidInput(char myChar, int myInt);
char playerToPlayerMove(int player);
int getHumanMove(int** board, int player);

class ConnectNBoard {
    private:
        int moves;
        int** board;
        int whichPlayerWin;
    
    public:
        int getMoves() {
            return moves;
        }

        int** getBoard() {
            return board;
        }

        int getWhichPlayerWin() {
            return whichPlayerWin;
        }

        ConnectNBoard(int flag) : moves(0), whichPlayerWin(0) {
            board = (int**) calloc(BOARD_SIZE, sizeof(int *));
            for (int i = 0; i < BOARD_SIZE; i++) {
                board[i] = (int *) calloc(BOARD_SIZE, sizeof(int));
            }
            if (flag == RANDOM_INIT) {
                int moves[3] = {EMPTY, PLAYER1, PLAYER2};
                for (int i = 0; i < BOARD_SIZE; i++) {
                    for (int j = 0; j < BOARD_SIZE; j++) {
                        board[i][j] = moves[random(2)];
                        if (board[i][j] != EMPTY) {
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
                    printf(" %c ", playerToPlayerMove(board[i][j]));
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
            // Check horizontal
            int consec = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == player) {
                        consec++;
                        if (consec == WIN_LENGTH) {
                            printf("player%d: Win by horizontal\n", player);
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
                    if (board[j][i] == player) {
                        consec++;
                        if (consec == WIN_LENGTH) {
                            printf("player%d: Win by vertical\n", player);
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
                        if (board[i+k][j+k] == player) {
                            consec++;
                            if (consec == WIN_LENGTH) {
                                printf("player%d: Win by right diagonal\n", player);
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
                    max_inspect = min(BOARD_SIZE-i-1, j) + 1;
                    // printf("i:%d, j:%d, max_inspect:%d\n", i, j, max_inspect);
                    for (int k = 0; k < max_inspect; k++) {
                        if (board[i+k][j-k] == player) {
                            consec++;
                            if (consec == WIN_LENGTH) {
                                printf("player%d: Win by left diagonal\n", player);
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

            printf("player%d: Not win\n", player);
            return false;
        }

        int makeMove(int i, int j, int player) {
            int opponent;
            if (player == PLAYER1) {
                opponent = PLAYER2;
            } else if (player == PLAYER2) {
                opponent = PLAYER1;
            } else {
                printf("Wrong player\n");
                return -1;
            }

            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                printf("The board is full\n");
                return -1;
            }
            if (i < 0 || i >= BOARD_SIZE
            || j < 0 || j >= BOARD_SIZE) {
                printf("Invalid move of i: %d, j: %d in makeMove\n", i, j);
                whichPlayerWin = opponent;
                return -1;
            }
            if (board[i][j] == EMPTY) {
                board[i][j] = player;
                moves++;
                return (i*BOARD_SIZE + j);
            } else {
                printf("Stone exists at i: %d, j: %d in makeMove\n", i, j);
                whichPlayerWin = opponent;
                return -1;
            }

        }

        int makeMove(int move, int player) {
            int opponent;
            if (player == PLAYER1) {
                opponent = PLAYER2;
            } else if (player == PLAYER2) {
                opponent = PLAYER1;
            } else {
                printf("Wrong player\n");
                return -1;
            }

            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                printf("The board is full\n");
                return -1;
            }
            if (move < 0 || move >= BOARD_SIZE * BOARD_SIZE) {
                printf("Invalid move of %d in makeMove\n", move);
                whichPlayerWin = opponent;
                return -1;
            }
            int i = move / BOARD_SIZE;
            int j = move % BOARD_SIZE;
            if (board[i][j] == EMPTY) {
                board[i][j] = player;
                moves++;
                return (i*BOARD_SIZE + j);
            } else {
                printf("Stone exists at i: %d, j: %d in makeMove\n", i, j);
                whichPlayerWin = opponent;
                return -1;
            }
        }

        int makeRandomMove(int player) {
            int opponent;
            if (player == PLAYER1) {
                opponent = PLAYER2;
            } else if (player == PLAYER2) {
                opponent = PLAYER1;
            } else {
                printf("Wrong player\n");
                return -1;
            }
            if (moves >= BOARD_SIZE * BOARD_SIZE) {
                return -1;
            }
            int i,j;
            i = random(BOARD_SIZE-1);
            j = random(BOARD_SIZE-1);
            while (board[i][j] != EMPTY) {
                i = random(BOARD_SIZE-1);
                j = random(BOARD_SIZE-1);
            }
            board[i][j] = player;
            moves++;
            return i*BOARD_SIZE + j;
        }

        void playGame(int (*player1Func)(int**, int), int (*player2Func)(int**, int), bool showBoard) {
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
                    if (myMove == -1)
                        break;
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
                    if (myMove == -1)
                        break;
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

bool checkValidInput(char myChar, int myInt) {
    int offset = myChar - 'A';
    if (offset < 0 || offset > BOARD_SIZE
    || myInt < 0 || myInt > BOARD_SIZE) {
        return false;
    }
    return true;
}

char playerToPlayerMove(int player) {
    if (player == EMPTY)
        return ' ';
    else if (player == PLAYER1)
        return PLAYER1MOVE;
    else if (player == PLAYER2)
        return PLAYER2MOVE;
    assert(0);
}

int getHumanMove(int** board, int player) {
    char myChar;
    int myInt;
    int myMove;
    int i,j;
    printf("player%d turn: ", player);
    while (true) {
        if (scanf("%c%d", &myChar, &myInt) == 2) {
            if (checkValidInput(myChar, myInt) == false) {
                printf("Invalid move, try again\n");
                continue;
            }
            myMove = charToMove(myChar, myInt);
            i = myMove / BOARD_SIZE;
            j = myMove % BOARD_SIZE;
            if (board[i][j] != EMPTY) {
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