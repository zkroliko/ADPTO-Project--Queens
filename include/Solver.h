#ifndef ADPTO_SOLVER_H
#define ADPTO_SOLVER_H


#include "Board.h"

class Solver {
    Board& board;
public:

    Solver(Board &board) : board(board) { }

    Board &getBoard() const {
        return board;
    }

    void setBoard(Board &board) {
        Solver::board = board;
    }

    void solve() {;}
};


#endif //ADPTO_SOLVER_H
