#include <utility>
#include "../include/Queen.h"
#include "../include/Board.h"

Board::Board(unsigned short size) : size(std::min(MAX_SIZE,size)) {}

void Board::addQueen(const Queen& queen, const unsigned short x, const unsigned short y) {
    Board::queens[Pos(x,y)] = queen;
}

