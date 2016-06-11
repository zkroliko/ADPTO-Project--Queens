#include <iostream>
#include "../include/Loader.h"

using namespace std;

int main() {
    int solution_size, board_size;
    /* Getting the problem specification */
    cin >> board_size;
    cout << "Board size: " << board_size << endl;
    cin >> solution_size;
    cout << "Solving solution of size: " << solution_size << endl;
    /* Getting the board */
//    Loader::load(new char**[]);
    Board board(8);

    Queen queen(1);
    board.addQueen(queen,1,1);
    return 0;
}