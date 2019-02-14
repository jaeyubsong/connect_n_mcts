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


class ConnectNBoard:
    def __init__(self, flag):
        self.moves = 0
        self.board = np.zeros(shape=(BOARD_SIZE, BOARD_SIZE))
        self.whichPlayerWin = 0
        if flag == RANDOM_INIT:
            moves = [EMPTY, PLAYER1, PLAYER2];
            for i in range(BOARD_SIZE):
                for j in range(BOARD_SIZE):
                    self.board[i][j] = moves[randbelow(3)]
                    if self.board[i][j] != EMPTY:
                        self.moves += 1

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
                print(" %c " % playerToPlayerMove(self.board[i][j]), end='')  # change required
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
                max_inspect = min(BOARD_SIZE-i-1, j) + 1
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
            print("Invalid move of %d in makeMove" % move)
            self.whichPlayerWin = opponent
            return -1
        i = int(move / BOARD_SIZE)
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
        i = randbelow(BOARD_SIZE)
        j = randbelow(BOARD_SIZE)
        while self.board[i][j] != EMPTY:
            i = randbelow(BOARD_SIZE)
            j = randbelow(BOARD_SIZE)
        self.board[i][j] = player
        self.moves += 1
        return i * BOARD_SIZE + j

    def playGame(self, player1Func, player2Func, showBoard):
        gameDone = False
        whoseTurn = PLAYER1
        player1Param = (self.board, PLAYER1)
        player2Param = (self.board, PLAYER2)
        while True:
            if self.moves >= BOARD_SIZE ** 2:
                break
            if whoseTurn == PLAYER1:
                myMove = player1Func(*player1Param)
                myMove = self.makeMove(myMove, PLAYER1)
                gameDone = self.isWin(PLAYER1)
                if myMove == -1:
                    break
                if showBoard == True:
                    self.showBoard()
                if gameDone == True:
                    break
                whoseTurn = PLAYER2
            elif whoseTurn == PLAYER2:
                myMove = player2Func(*player2Param)
                myMove = self.makeMove(myMove, PLAYER2)
                gameDone = self.isWin(PLAYER2)
                if myMove == -1:
                    break
                if showBoard == True:
                    self.showBoard()
                if gameDone == True:
                    break
                whoseTurn = PLAYER1

    def finishGame(self, printWinner):
        if printWinner == True:
            if self.whichPlayerWin == PLAYER1:
                print("Player1 won")
            elif self.whichplayerWin == PLAYER2:
                print("Player2 won")
            else:
                print("No one won")
        return self.whichPlayerWin



def printMove(myMove):
    startChar = ord('A')
    startInt = BOARD_SIZE
    startChar += myMove % BOARD_SIZE
    startInt -= myMove / BOARD_SIZE
    print("%c%d" % (chr(startChar), startInt), end='')


def charToMove(myChar, myInt):
    offset = ord(myChar) - ord('A')
    print("myChar:" + myChar + " myInt:" + str(myInt))
    return (BOARD_SIZE - myInt) * BOARD_SIZE + offset


def checkValidInput(myChar, myInt):
    offset = ord(myChar) - ord('A')
    if offset < 0 or offset > BOARD_SIZE or myInt < 0 or myInt > BOARD_SIZE:
        return False
    return True


def playerToPlayerMove(player):
    if player == EMPTY:
        return ' '
    elif player == PLAYER1:
        return PLAYER1MOVE
    elif player == PLAYER2:
        return PLAYER2MOVE

def getHumanMove(board, player):
    while True:
        print("Player%d turn: " % player, end='')
        x = input()
        if len(x) < 2:
            print("Invalid move (too short), try again")
            continue
        myChar = x[0]
        myInt = x[1:]
        if not myInt.isdigit():
            print("Invalid move (not integer), try again")
            continue
        if ord(myChar) < ord('A') or ord(myChar) > ord('Z'):
            print("Invalid move (wrong character), try again")
            continue
        myInt = int(myInt)
        myMove = charToMove(myChar, myInt)
        i = int(myMove / BOARD_SIZE)
        j = myMove % BOARD_SIZE
        # print("i:" + str(i) + " and j:" + str(j))
        if board[i][j] != EMPTY:
            print("Invalid move, try again")
            continue
        return myMove



print("Hello from connect n")
gameboard = ConnectNBoard(EMPTY_INIT)
gameboard.showBoard()
gameboard.playGame(getHumanMove, getHumanMove, True)
gameboard.finishGame(True)
