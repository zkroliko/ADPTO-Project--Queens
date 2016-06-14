#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include "../include/Loader.h"
#include "../include/Debugging.h"

using namespace std;

Board* Loader::load(const unsigned short size) {
    if (size < MIN_QUEEN_POWER || size > MAX_QUEEN_POWER) {
        __throw_invalid_argument("Invalid board size");
    }
    Board *board = new Board(size);

    std::string line;
    unsigned short power;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            cin >> line;
            power = static_cast<unsigned short>(Queen::powerFromExternal(std::stoull(line)));
            board->addQueen(Queen(power),i,j);
        }
    }

    return board;
}