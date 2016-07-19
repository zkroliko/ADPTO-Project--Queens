#ifndef ADPTO_BOARD_H
#define ADPTO_BOARD_H
#include<map>
#include "../include/Queen.h"
#include "Queen.h"
#include "Postition.h"

const unsigned short MAX_BOARD_SIZE = 128;

class Queen;

typedef std::map<Pos, Queen*> PlacementMap;
typedef std::map<unsigned short, unsigned short> QueenCount;

enum Axes
{
    horizontal = 0, vertical = 1, diagonal_left = 2, diagonal_right = 3
};

class Board {
private:
    unsigned short size;
    PlacementMap queens;
    QueenCount rowQueenCount;
    QueenCount columnQueenCount;
    QueenCount leftDiagonalQueenCount;
    QueenCount rightDiagonalQueenCount;
public:
    Board() {size = 0;}
    Board(unsigned short size);
    void addQueen(Queen& queen, const unsigned short x, const unsigned short y);
    bool changeQueen(unsigned short x, unsigned short y, unsigned short power);
    bool removeQueen(unsigned short x, unsigned short y);
    bool occupied(unsigned short x, unsigned short y) { return queens.count(Pos(x,y)) > 0; }
    Queen* get(unsigned short x, unsigned short y) { return queens.at(Pos(x,y));}
    short getSize() const { return size;}
    std::string toString();
    std::string queenCountsToString();
    PlacementMap* getQueens() { return &queens; }
    unsigned short limitOfMovesToPosition(const Pos& pos);
    inline unsigned short posToAxis(const Pos& pos, const Axes);
    inline unsigned short posToRow(const Pos& pos);
    inline unsigned short posToColumn(const Pos& pos);
    inline unsigned short posToLeftDiagonal(const Pos& pos);
    inline unsigned short posToRightDiagonal(const Pos& pos);

    void incrementQueenCounts(const Pos& pos);
    void decrementQueenCounts(const Pos& pos);
};

#endif //ADPTO_BOARD_H
