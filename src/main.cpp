#include <iostream>
#include "../include/Board.h"

using namespace std;


int main() {
    cout << "Hello, World!" << endl;
    Board board(8);
    Queen queen(1);
    board.addQueen(queen,1,1);
    cout << "Board size: " << board.getSize();
    return 0;
}