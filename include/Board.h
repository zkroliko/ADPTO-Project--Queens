#ifndef ADPTO_BOARD_H
#define ADPTO_BOARD_H
#include<map>
#include "../include/Queen.h"

const unsigned short MAX_SIZE = 128;

typedef std::tuple<unsigned short,unsigned short> Pos;

class Board {
private:
    unsigned short size;
    std::map<Pos, Queen*> queens;
public:
    Board() {}
    Board(unsigned short size);
    void addQueen(Queen& queen, const unsigned short x, const unsigned short y);
    const bool occupied(unsigned short x, unsigned short y) { return queens.count(Pos(x,y)) > 0; }
    const Queen& get(unsigned short x, unsigned short y) { return *queens.at(Pos(x,y));}
    short getSize() const { return size;}
    std::string toString();
};

#endif //ADPTO_BOARD_H
