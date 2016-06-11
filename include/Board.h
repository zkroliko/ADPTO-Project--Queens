#ifndef ADPTO_BOARD_H
#define ADPTO_BOARD_H
#include<map>
#include "../include/Queen.h"

const unsigned short MAX_SIZE = 128;

typedef std::tuple<unsigned short,unsigned short> Pos;

class Board {
private:
    unsigned short size;
    std::map<Pos, Queen> queens;
public:
    Board() {}
    Board(unsigned short size);
    void addQueen(const Queen& queen, const unsigned short x, const unsigned short y);
    short getSize() { return size;}
};

#endif //ADPTO_BOARD_H
