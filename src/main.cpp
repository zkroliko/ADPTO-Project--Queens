#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include "../include/Loader.h"

#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
    #define DEBUG(x) std::cerr << x << endl;
#else
    #define DEBUG(x)
#endif

using namespace std;

int main() {
    int solution_size;
    unsigned short board_size;
    /* Getting the problem specification */

    cin >> board_size;
    DEBUG("Board size: " << board_size);
    cin >> solution_size;
    DEBUG("Solving solution of size: " << solution_size);
    // Getting the board into the loader
    std::string line;
    istringstream iss(line);
    std::vector<std::string> lines;
    for(int i = 0; i < board_size; ++i)
        cin >> line;
        lines.push_back(line);
    Board* problem = Loader::load(board_size,lines);
    Board board(8);

    Queen queen(1);
    board.addQueen(queen,1,1);
    return 0;
}