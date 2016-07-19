#ifndef ADPTO_SOLVER_H
#define ADPTO_SOLVER_H


#include <vector>
#include "Board.h"
#include "Actions.h"

typedef std::vector<Queen*> QueenVector;
typedef std::vector<Move> MoveVector;

const unsigned short POWER_WEIGHT = 1;
const unsigned short CONNECTION_COUNT_WEIGHT = 2;

class Solver {
    Board board;
    QueenVector leftQueens;
    QueenVector usedQueens;
    MoveVector moves;
    int queenCount;
    int target;
public:
    Solver(Board& board) : board(board) { queenCount =0; target=0; }
    void setBoard(Board &board) { Solver::board = board;}
    bool possible(unsigned int target);
    const MoveVector *getSolution() const {
        return &moves;
    }
private:
    void move(Queen *source, Queen *target);
    void undoMove();
    unsigned int countQueens();
    void outlineQueens();
    void sortQueens();
    bool check();
    void insertJoined(Queen *source, Queen *target);
};


#endif //ADPTO_SOLVER_H
