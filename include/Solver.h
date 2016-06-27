#ifndef ADPTO_SOLVER_H
#define ADPTO_SOLVER_H


#include <vector>
#include "Board.h"
#include "Move.h"

class Solver {
    Board board;
    std::vector<Move*> moves;
    unsigned int queenCount;
public:
    Solver(Board& board) : board(board) {  }

    void setBoard(Board &board) {
        Solver::board = board;
    }

    bool possible(unsigned int target);

    const std::vector<Move*> *getSolution() const {
        return &moves;
    }
private:
    unsigned int kernelize(unsigned int target);
    bool moveValid(const Queen& source, const Queen& target) const;
    void move(Queen *source, Queen *target);
    void undo();
    unsigned int countQueens();

    bool check();
};


#endif //ADPTO_SOLVER_H
