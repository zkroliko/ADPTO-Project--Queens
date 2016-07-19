#include <algorithm>
#include <iostream>
#include "../include/Solver.h"
#include "../include/Debugging.h"
#include "../include/Tools.h"

bool Solver::possible(unsigned int solutionRequirement) {
    target = solutionRequirement;
    queenCount = countQueens();
    outlineQueens();
    DEBUG("Solver: We have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    DEBUG("Now running recursive function.");
    return check();
}

bool Solver::check() {
//    std::cout << "Limit: " << endl;
//    std::cout << board.queenCountsToString();
    if (queenCount <= target) {
        return true;
    }
    sortQueens();

    for (Queen *current : leftQueens) {
        if (current->doesExist()) {
            QueenVector possibilities;
            for (auto connection : *current->getConnections()) {
                Queen *possibility = Queen::findActiveQueen(current, connection.second);
                if (possibility && current->canJoin(*possibility)) {
                    possibilities.push_back(possibility);
                }
            }
            for (auto possibility : possibilities) {
                    move(current, possibility);
                    if (check()) {
                        return true;
                    }
            }
        }
    }
    undoMove();
    return false;
}

void Solver::move(Queen *source, Queen *target) {
    Move move(source,target);
    target -= move.apply(board);
    moves.push_back(move);
    queenCount--;

//    leftQueens.erase(source)
}

void Solver::undoMove() {
    target += moves.back().undo(board);
    moves.pop_back();
    queenCount++;
}

unsigned int Solver::countQueens() {
    unsigned int count = 0;

    for (auto entry : *board.getQueens()) {
        if (entry.second->doesExist()) {
            count++;
        }
    }
    return count;
}

void Solver::outlineQueens() {
    QueenVector retrieved;
    mapToVec<PlacementMap,QueenVector>(*board.getQueens(),retrieved);
    for (QueenVector::iterator it = retrieved.begin(); it != retrieved.end(); ++it) {
        if (it.operator*()->doesExist()) {
            leftQueens.push_back(it.operator*());
        }
    }
}

void Solver::sortQueens() {
    std::sort(leftQueens.begin(),leftQueens.end(), [](Queen* lhs, Queen* rhs )
    {
        return lhs->rating() < rhs->rating();
    });
}






















