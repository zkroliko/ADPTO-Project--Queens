#include <utility>
#include "../include/Queen.h"
#include "../include/Board.h"

Board::Board(short size) : size(std::min(MAX_SIZE,size)) {}

void Board::addQueen(const Queen& queen, const short x, const short y) {
    Board::queens[Pos(x,y)] = queen;
}

