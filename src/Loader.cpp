#include "../include/Loader.h"


Board* Loader::load(const short size, const char* input[]) {
    Board *board = new Board(size);

    return board;
}