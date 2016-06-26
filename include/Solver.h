#ifndef ADPTO_SOLVER_H
#define ADPTO_SOLVER_H


#include <vector>
#include "Board.h"

class Solver {
    Board& board;
    std::vector<Move> moves;
public:
    Solver(Board &board) : board(board) {  }

    Board &getBoard() const {
        return board;
    }

    void setBoard(Board &board) {
        Solver::board = board;
    }

    bool possible(unsigned int target);

    const std::vector<Move> &getSolution() const {
        return moves;
    }
private:
    unsigned int kernelize(unsigned int target);
};


#endif //ADPTO_SOLVER_H
