#include <utility>
#include "../include/Queen.h"
#include "../include/Board.h"

Board::Board(unsigned short size) : size(std::min(MAX_BOARD_SIZE,size)) {}

void Board::addQueen(Queen& queen, const unsigned short x, const unsigned short y) {
    Board::queens[Pos(x,y)] = &queen;
//incrementQueenCounts(Pos(x,y));
}

std::string Board::toString() {
    std::string result;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            if (queens.count(Pos(i,j)) > 0 && queens[Pos(i, j)]->doesExist()){
                result += std::to_string(Queen::powerToExternal(queens[Pos(i,j)]->getPower()));
            } else {
                result += "0";
            }
            result += "    ";
        }
        result += "\n";
    }
    return result;
}

bool Board::removeQueen(unsigned short x, unsigned short y) {
    if (queens.count(Pos(x,y)) > 0 ) {
        queens[Pos(x,y)]->setExists(false);
//        decrementQueenCounts(Pos(x,y));
        return true;
    } else {
        return false;
    }
}

bool Board::changeQueen(unsigned short x, unsigned short y, unsigned short power) {
    if (queens.count(Pos(x,y)) >0 ) {
        queens[Pos(x,y)]->setPower(power);
        return false;
    } else {
        return false;
    }
}

unsigned short Board::posToRow(const Pos &pos) {
    return std::get<0>(pos);
}

unsigned short Board::posToColumn(const Pos &pos) {
    return std::get<1>(pos);
}

unsigned short Board::posToLeftDiagonal(const Pos &pos) {
    return static_cast<unsigned short>(std::get<0>(pos) - std::get<1>(pos) + size - 1);
}

unsigned short Board::posToRightDiagonal(const Pos &pos) {
    return static_cast<unsigned short>(2*size-2-std::get<0>(pos) - std::get<1>(pos));
}

unsigned short Board::posToAxis(const Pos &pos, const Axes axis) {
    switch( axis )
    {
        case horizontal:
            return posToRow(pos);
        case vertical:
            return posToColumn(pos);
        case diagonal_left:
            return posToLeftDiagonal(pos);
        case diagonal_right:
            return posToRightDiagonal(pos);

        default:
            std::__throw_logic_error("No such axis");
    }
}

void Board::incrementQueenCounts(const Pos &pos) {
    QueenCount* axes[] = {&rowQueenCount,&columnQueenCount,&leftDiagonalQueenCount,&rightDiagonalQueenCount};
    for (unsigned int i = 0; i < 4; ++i) {
        if (axes[i]->count(posToAxis(pos,Axes(i))) > 0) {
            (*axes[i])[posToAxis(pos,Axes(i))]++;
        } else {
            (*axes[i])[posToAxis(pos,Axes(i))] = 1;
        }
    }
}

void Board::decrementQueenCounts(const Pos &pos) {
    QueenCount* axes[] = {&rowQueenCount,&columnQueenCount,&leftDiagonalQueenCount,&rightDiagonalQueenCount};
    for (unsigned int i = 0; i < 4; ++i) {
        if (axes[i]->count(posToAxis(pos,Axes(i))) > 0) {
            (*axes[i])[posToAxis(pos,Axes(i))]--;
        } else {
            std::__throw_logic_error("Decrementing should only be done after incrementing - probably there is no such queen");
        }
    }
}

unsigned short Board::limitOfMovesToPosition(const Pos& pos) {
    unsigned short total = 0;
    QueenCount* axes[] = {&rowQueenCount,&columnQueenCount,&leftDiagonalQueenCount,&rightDiagonalQueenCount};
    for (unsigned int i = 0; i < 4; ++i) {
        if (axes[i]->count(posToAxis(pos,Axes(i))) > 0) {
            total += (*axes[i])[posToAxis(pos,Axes(i))];
        }
    }
    return total;
}























