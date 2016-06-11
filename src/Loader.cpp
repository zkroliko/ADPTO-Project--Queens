#include <vector>
#include "../include/Loader.h"


Board* Loader::load(const unsigned short size, const std::vector<std::string> lines) {
    Board *board = new Board(size);
    for (auto line : lines) {

    }

    return board;
}