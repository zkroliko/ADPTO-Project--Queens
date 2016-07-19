#ifndef ADPTO_MOVE_H
#define ADPTO_MOVE_H

#include "Queen.h"
#include "Board.h"

using namespace std;

class Move;
class Ignore;

typedef vector<Ignore*> ReductionVector;

class Action {
public:
    virtual short apply(Board& board) = 0;
    virtual short undo(Board& board) = 0;
    virtual string toString() = 0;
    virtual bool isIgnore() = 0;
};

class Move: public Action {
    Queen* source;
    Queen* target;
    ReductionVector reductions;
public:
    Move() { }
    Move(Queen *source, Queen *target) : source(source), target(target) { }
    virtual short apply(Board& board);
    virtual short undo(Board& board);
    virtual string toString();
    virtual bool isIgnore() { return false; }
    Queen *getSource() const { return source;}
    Queen *getTarget() const { return target;}

};

class Ignore: public Action {
    Queen* queen;
public:
    Ignore(Queen *queen) : queen(queen) { }
    virtual short apply(Board& board);
    virtual short undo(Board& board);
    virtual bool isIgnore() { return true; }
    Queen *getQueen() const { return queen;}
};

#endif //ADPTO_MOVE_H
