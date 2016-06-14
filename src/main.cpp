#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include "../include/Loader.h"
#include "../include/Debugging.h"

using namespace std;

int main() {
    int solution_size;
    unsigned short board_size;
    /* Getting the problem specification */

    cin >> board_size;
    DEBUG("Board size: " << board_size);
    cin >> solution_size;
    DEBUG("Solving solution of size: " << solution_size);
    // Getting the board from the loader
    DEBUG("Loading board");
    Board* problem = Loader::load(board_size);
    DEBUG("Loading board finished");
    cout << problem->toString();
//    Queen queen(1);
//    board.addQueen(queen,1,1);
    return 0;
}