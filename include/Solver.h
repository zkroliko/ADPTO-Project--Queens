#ifndef ADPTO_SOLVER_H
#define ADPTO_SOLVER_H


#include <vector>
#include "Board.h"
#include "Move.h"

typedef std::vector<Queen*> QueenVector;
typedef std::vector<Move*> MoveVector;

class Solver {
    Board board;
    QueenVector leftQueens;
    MoveVector moves;
    unsigned int queenCount;
    unsigned int target;
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
    void outlineQueens();
    void sortQueens();
    bool check();
};


#endif //ADPTO_SOLVER_H
