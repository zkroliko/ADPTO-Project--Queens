#include <iostream>
#include "../include/Loader.h"

#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
    #define DEBUG(x) std::cerr << x << endl;
#else
    #define DEBUG(x)
#endif

using namespace std;

int main() {
    int solution_size, board_size;
    /* Getting the problem specification */
    cin >> board_size;
    DEBUG("Board size: " << board_size);
    cin >> solution_size;
    DEBUG("Solving solution of size: " << solution_size);
    /* Getting the board */
    //Allocating
    int** board_chars = new int*[board_size];
    for(int i = 0; i < board_size; ++i)
        board_chars[i] = new int[board_size];
    DEBUG("Board allocated");

    //Deallocating
    for(int i = 0; i < board_size; ++i)
        delete board_chars[i];
    delete board_chars;
    DEBUG("Board deallocated");
//    Loader::load(new char**[]);
    Board board(8);

    Queen queen(1);
    board.addQueen(queen,1,1);
    return 0;
}