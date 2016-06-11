#ifndef ADPTO_BOARD_H
#define ADPTO_BOARD_H
#include<map>
#include "../include/Queen.h"

const short MAX_SIZE = 128;

typedef std::tuple<short,short> Pos;

class Board {
private:
    short size;
    std::map<Pos, Queen> queens;
public:
    Board() {}
    Board(short size);
    void addQueen(const Queen& queen, const short x, const short y);
    short getSize() { return size;}
};

#endif //ADPTO_BOARD_H
