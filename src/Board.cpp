#include <utility>
#include "../include/Queen.h"
#include "../include/Board.h"

Board::Board(unsigned short size) : size(std::min(MAX_SIZE,size)) {}

void Board::addQueen(const Queen& queen, const unsigned short x, const unsigned short y) {
    Board::queens[Pos(x,y)] = queen;
}

std::string Board::toString() {
    std::string result;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            if (queens.count(Pos(i,j)) >0 ){
                result += std::to_string(Queen::powerToExternal(queens[Pos(i,j)].getPower()));
            } else {
                result += " ";
            }
            result += "    ";
        }
        result += "\n";
    }
    return result;
}



