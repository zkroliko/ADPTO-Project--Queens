#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include "../include/Loader.h"
#include "../include/Debugging.h"
#include "../include/Solver.h"
#include "../include/Move.h"

using namespace std;

int main() {
    unsigned int solution_size;
    unsigned short board_size;
    /* Getting the problem specification */

    cin >> board_size;
    DEBUG("Board size: " << board_size);
    cin >> solution_size;
    DEBUG("Will be solving solution of size: " << solution_size);
    // Getting the board from the loader
    DEBUG("Loading board");
    Board* problem = Loader::load(board_size);
    DEBUG("Loading board finished");
    DEBUG("The board:");
    DEBUG(problem->toString());
    DEBUG("Searching for a solution of size: " << solution_size);
    Solver solver(*problem);
    if (solver.possible(solution_size)) {
        DEBUG("Solution found");
        for (Move* move : *solver.getSolution()) {
            cout << std::get<0>(std::get<0>(*move)->getPosition()) << " " << std::get<1>(std::get<0>(*move)->getPosition());
            cout << " ";
            cout << std::get<0>(std::get<1>(*move)->getPosition()) << " " << std::get<1>(std::get<1>(*move)->getPosition());
            cout << endl;
        }
    } else {
        DEBUG("Solution not found");
    }

    return 0;
}