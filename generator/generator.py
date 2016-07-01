#!/usr/bin/env python
# coding=utf-8

# generator.py

import random
from sys import *

PRINT_MOVES = True

MAX_TRIES_FOR_TARGET = 10

MAX_TOTAL_EXPONENT = 25  # 60
MAX_BOARD_SIZE = 128

directions = {
    "top_left": (-1, -1),
    "top": (0, -1),
    "top_right": (1, -1),
    "right": (1, 0),
    "bottom_right": (1, 1),
    "bottom": (0, 1),
    "bottom_left": (-1, 1),
    "left": (-1, 0)
}


class Board():
    def __init__(self, size):
        self.size = size
        self.queens = {}

    def printOut(self):
        M = max(pow(2,power) for queen, power in self.queens.iteritems())
        L = len(str(2 ^ M)) + 1
        for row in range(0, self.size):
            r = ""
            for cell in range(0, self.size):
                x = str(pow(2, self.queens[(row, cell)])) if (row, cell) in self.queens else "0"
                r += x + " " * (L - len(x) + 1)
            print r
        print ""

    # Can move
    def move(self, source):
        if self.queens[source] <= 0:
            return False
        for direction, offset in directions.iteritems():
            possibility = self.findMove(source, direction)
            if possibility:
                self.queens[source] -= 1
                self.queens[possibility] = self.queens[source]
                return possibility
        return None

    # Find move in direction
    def findMove(self, source, direction):
        x, y = source[0] + directions[direction][0], source[1] + directions[direction][1]
        possible = []
        while (x, y) not in self.queens and 0 <= x < self.size and 0 <= y < self.size:
            possible.append((x, y))
            x += directions[direction][0]
            y += directions[direction][1]

        if len(possible) > 0:
            return possible[int(random.uniform(0, len(possible)))]
        else:
            return None


# Generate initial points

def createInitial(target, board):
    initial = board.queens
    count = 0
    powerLeft = MAX_TOTAL_EXPONENT
    while len(initial) < target:
        x = int(random.uniform(0, board.size))
        y = int(random.uniform(0, board.size))
        power = int(random.triangular(0, powerLeft))
        powerLeft -= power
        if (x, y) not in initial:
            initial[(x, y)] = power


# Creates the problem

def divide(board, minimalMoves):
    checked = set()
    moves = []
    while len(moves) < minimalMoves and len(checked) < len(board.queens):
        queen = board.queens.items()[int(random.uniform(0, len(board.queens)))][0]
        while queen in checked:
            queen = board.queens.items()[int(random.uniform(0, len(board.queens)))][0]
        if queen not in checked:
            move = board.move(queen)
            while move and len(moves) < minimalMoves:
                moves.append((move,queen))
                move = board.move(queen)
            if move:
                moves.append((move,queen))
                # Queen is moving here
            checked.add(queen)
    return moves if len(moves) >= minimalMoves else []

# Printing moves

def printMoves(moves):
    moves.reverse()
    for move in moves:
        print("%s %s %s %s" % (move[0][0], move[0][1], move[1][0], move[1][1]))


# Main

def main():
    if (len(argv) != 4):
        print("USE: generator.py <board size> <queen count> <targetMoves>")
        exit(-1)
    queenTarget = int(argv[2])
    boardSize = int(argv[1])
    if (boardSize not in range(1, MAX_BOARD_SIZE)):
        print("%s is a bad board size, range is (0,%s)" % (boardSize, MAX_BOARD_SIZE))
        exit(-1)

    target_moves = int(argv[3])
    while (target_moves > 0):
        found = False
        tries = 0
        while not found and tries < MAX_TRIES_FOR_TARGET:
            board = Board(int(argv[1]))
            createInitial(queenTarget, board)
            found = divide(board, target_moves)
            if len(found) > 0:
                # FOUND THE RESULT
                print board.size
                print queenTarget
                board.printOut()
                if PRINT_MOVES:
                    printMoves(found)
                return 0
            tries -= 1
        target_moves -= 1


main()
