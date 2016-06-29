#include <algorithm>
#include "../include/Solver.h"
#include "../include/Debugging.h"
#include "../include/Tools.h"

bool Solver::possible(unsigned int solutionRequirement) {
    target = solutionRequirement;
    queenCount = countQueens();
    outlineQueens();
    DEBUG("Solver: We have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    DEBUG(board.toString());
    DEBUG("Now running recursive function.");
    return check();
}

bool Solver::check() {
    if (queenCount <= target) {
        return true;
    }
    sortQueens();
    for (Queen *current: leftQueens) {
        if (current->doesExist()) {
            for (auto connection : *current->getConnections()) {
                Queen *possibility = Queen::findActiveQueen(current, connection.second);
                if (possibility && current->canJoin(*possibility)) {
                    // There is a viable connection to this direction
                    move(current, possibility);
                    if (check()) {
                        return true;
                    } else {
                        undoMove();
                    }
                }
            }
            // No viable connection
        }
    }
    return false;
}

///* Uses above function to reduce some problems */
//void Solver::ignoreUselessNeighbours(Queen *queen) {
//    for (auto connection: *queen->getConnections()) {
//        Queen* other = connection.second;
//        if (other->doesExist()) {
//            if (queen->doesExist()) {
//                if (queen->getPower() > other->getPower() && other->useless()) {
//                    ignore(other);
//                }
//            } else {
//                if (other->useless()) {
//                    ignore(other);
//                }
//            }
//        }
//    }
//}

void Solver::move(Queen *source, Queen *target) {
    Move move(source,target);
    target -= move.apply();
    moves.push_back(move);
//    ignoreUselessNeighbours(source);
//    ignoreUselessNeighbours(target);
    queenCount--;
}

void Solver::undoMove() {
    target += moves.back().undo();
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
    std::sort(leftQueens.begin(),leftQueens.end(), [ ]( Queen* lhs, Queen* rhs )
    {
        unsigned short left = lhs->getPower()*POWER_WEIGHT+lhs->connectionCount()*CONNECTION_COUNT_WEIGHT;
        unsigned short right = rhs->getPower()*POWER_WEIGHT+rhs->connectionCount()*CONNECTION_COUNT_WEIGHT;
        return left < right;
    });
}


















