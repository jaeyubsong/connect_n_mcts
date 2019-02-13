import numpy as np
from secrets import randbelow
import random

EMPTY = 0
PLAYER1 = 1
PLAYER2 = 2

BOARD_SIZE = 5
WIN_LENGTH = 3

PLAYER1MOVE = 'O'
PLAYER2MOVE = 'X'

EMPTY_INIT = 0
RANDOM_INIT = 1


def myRandom(x):
    start = time.time()
    random_numbers = []
    for _ in range(x):
        random_numbers.append(os.urandom(1))
    end = time.time()
    print(end - start)


class ConnectNBoard:
    def __init__(self):
        self.moves = 0
        self.board = np.zeros(shape=(BOARD_SIZE, BOARD_SIZE))
        self.whichPlayerWin = 0

    def showBoard(self):
        startChar = ord('A')
        startNum = BOARD_SIZE
        print("   ", end='')
        for i in range(BOARD_SIZE):
            print(" %c  " % (chr(startChar)), end='')
            startChar += 1
        print()
        for i in range(BOARD_SIZE):
            if startNum < 10:
                print(" ", end='')
            print("%d " % startNum, end='')
            startNum -= 1
            for j in range(BOARD_SIZE):
                print(" %c " % 'X', end='')  # change required
                if j != BOARD_SIZE - 1:
                    print("|", end='')
            print("\n   ", end='')
            for k in range(BOARD_SIZE):
                print("----", end='')
            print()

    def isWin(self, player):
        # Check horizontal
        consec = 0
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                if self.board[i][j] == player:
                    consec += 1
                    if consec == WIN_LENGTH:
                        print("player%d: Win by horizontal" % player)
                        self.whichPlayerWin = player
                        return True
                    else:
                        consec = 0
            consec = 0

        # Check vertical
        consec = 0
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                if self.board[j][i] == player:
                    consec += 1
                    if consec == WIN_LENGTH:
                        print("player%d: Win by vertical" % player)
                        self.whichPlayerWin = player
                        return True
                    else:
                        consec = 0
            consec = 0

        # Check right diagonal
        consec = 0
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                max_inspect = BOARD_SIZE - max(i, j)
                for k in range(max_inspect):
                    if self.board[i+k][j+k] == player:
                        consec += 1
                        if consec == WIN_LENGTH:
                            print("player%d: Win by right diagonal" % player)
                            self.whichPlayerWin = player
                            return True
                        else:
                            consec = 0
                consec = 0

        # Check left diagonal
        consec = 0
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                max_inspect = BOARD_SIZE - max(BOARD_SIZE - i, j)
                for k in range(max_inspect):
                    if self.board[i-k][j-k] == player:
                        consec += 1
                        if consec == WIN_LENGTH:
                            print("player%d: Win by left diagonal" % player)
                            self.whichPlayerWin = player
                            return True
                        else:
                            consec = 0
                consec = 0

        print("player%d: Not win" % player)
        return False

    def makeMove(self, i, j, player):
        if player == PLAYER1:
            opponent = PLAYER2
        elif player == PLAYER2:
            opponent = PLAYER1
        else:
            print("Wrong player")
            return -1

        if self.moves >= BOARD_SIZE ** 2:
            print("The board is full")
            return -1
        if i < 0 or i > BOARD_SIZE or j < 0 or j >= BOARD_SIZE:
            print("Invalid move of i: %d, j: %d in makeMove" % (i, j))
            self.whichPlayerWin = opponent
            return -1
        if self.board[i][j] == EMPTY:
            self.board[i][j] = player
            self.moves += 1
            return i * BOARD_SIZE + j
        else:
            print("Stone exists at i: %d, j: %d in makeMove" % (i, j))
            self.whichPlayerWin = opponent
            return -1

    def makeMove(self, move, player):
        if player == PLAYER1:
            opponent = PLAYER2
        elif player == PLAYER2:
            opponent = PLAYER1
        else:
            print("Wrong player")
            return -1

        if self.moves >= BOARD_SIZE * BOARD_SIZE:
            print("The board is full")
            return -1
        if move < 0 or move >= BOARD_SIZE ** 2:
            print("Invalid move of i: %d, j: %d in makeMove" % (i, j))
            self.whichPlayerWin = opponent
            return -1
        i = move / BOARD_SIZE
        j = move % BOARD_SIZE
        if self.board[i][j] == EMPTY:
            self.board[i][j] = player
            self.moves += 1
            return i * BOARD_SIZE + j
        else:
            print("Stone exists at i: %d, j: %d in makeMove" % (i, j))
            self.whichPlayerWin = opponent
            return -1

    def makeRandomMove(self, player):
        if player == PLAYER1:
            opponent = PLAYER2
        elif player == PLAYER2:
            opponent = PLAYER1
        else:
            print("Wrong player")
            return -1
        if self.moves >= BOARD_SIZE ** 2:
            return -1



print("Hello from connect n")
gameboard = ConnectNBoard()
gameboard.showBoard()