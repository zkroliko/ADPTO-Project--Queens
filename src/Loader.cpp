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

    // Loading and making the horizontal connection_count at the same time

    std::string element;
    unsigned short power;
    Queen *last = nullptr;

    int connection_count = 0;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            cin >> element;
            unsigned long long parsedLong = std::stoull(element);
            if (parsedLong > 0) {
                power = static_cast<unsigned short>(Queen::powerFromExternal(parsedLong));
                Queen* created = new Queen(power);
                board->addQueen(*created, i, j);
                if (last) {
                    last->addConnection(Direction::right, *created);
                    created->addConnection(Direction::left, *last);
                    connection_count++;
                }
                last = created;
            }
        }
        last = nullptr;
    }
    DEBUG("Horizontal connections added: " << connection_count );
    // Making the vertical connections
    connection_count = 0;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            if (board->occupied(j,i)) {
                Queen actual = board->get(j,i);
                if (last) {
                    last->addConnection(Direction::bottom, actual);
                    actual.addConnection(Direction::top, *last);
                    connection_count++;
                }
                last = &actual;
            }
        }
        last = nullptr;
    }
    DEBUG("Vertical connections added: " << connection_count );

    // Making the diagonal left to right connections
    connection_count = 0;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = i; j < size; ++j) {
            if (board->occupied(j, j+i)) {
                Queen actual = board->get(j,j+i);
                if (last) {
                    last->addConnection(Direction::bottom_right, actual);
                    actual.addConnection(Direction::top_left, *last);
                    connection_count++;
                }
                last = &actual;
            }
        }
        last = nullptr;
    }
    DEBUG("Diagonal left connections added: " << connection_count );

//    // Making the diagonal right to left connections
//    connection_count = 0;
//    for(unsigned short i = (unsigned short) (size - 1); i >= 0; --i) {
//        for(unsigned short j = i; j >= 0; --j) {
//            if (j+i < size && board->occupied(j, j+i)) {
//                Queen actual = board->get(j,j+i);
//                if (last) {
//                    last->addConnection(Direction::bottom_left, actual);
//                    actual.addConnection(Direction::top_right, *last);
//                    connection_count++;
//                }
//                last = &actual;
//            }
//        }
//        last = nullptr;
//    }
//    DEBUG("Diagonal right connections added: " << connection_count );

    return board;
}