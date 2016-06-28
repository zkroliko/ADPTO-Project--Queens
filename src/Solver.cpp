#include <algorithm>
#include "../include/Solver.h"
#include "../include/Debugging.h"
#include "../include/Tools.h"

bool Solver::possible(unsigned int solutionRequirement) {
    target = solutionRequirement;
    queenCount = countQueens();
    outlineQueens();
    DEBUG("Solver: We have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    kernelize();
    DEBUG("Solver: AFTER KERNELIZATION we have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    DEBUG("Solver: Board after kernelization:");
    DEBUG(board.toString());
    DEBUG("Now running recursive function.");
    return check();
}

bool Solver::check() {
    if (queenCount <= target) {
        return true;
    }
    sortQueens();
    for (Queen * queen: leftQueens) {
        if (queen->doesExist()) {
            for (auto connection : *queen->getConnections()) {
                if (queen->canJoin(*connection.second)) {
                    move(queen,connection.second);
                    if (check()) {
                        return true;
                    } else {
                        undoStep();
                    }
                }
            }
        }
    }
    return false;
}

void Solver::kernelize() {
    for (auto queen : leftQueens) {
        if (queen->getConnections()->empty()) {
            ignore(queen);
        }
    }
}

/*
 *  If at current time there is no neighbour with lower power - all have higher power,
 *  then we can assume that the current queen won't be useful for the solution
 *  because no chain of reduction can pass through it,
 *  therefore it is useless
 */
bool Solver::uselessToNeighbours(Queen *queen) {
    for (auto connection: *queen->getConnections()) {
        if (connection.second->getPower() <= queen->getPower()) {
            return false;
        }
    }
    return true;
}

/* Uses above function to reduce some problems */
void Solver::ignoreUselessNeighbours(Queen *queen) {
    for (auto connection: *queen->getConnections()) {
        if (connection.second->doesExist()) {
            if (queen->doesExist()) {
                if (queen->getPower() > connection.second->getPower() && uselessToNeighbours(connection.second)) {
                    ignore(connection.second);
                }
            } else {
                if (uselessToNeighbours(connection.second)) {
                    ignore(connection.second);
                }
            }
        }
    }
}


bool Solver::moveValid(const Queen &source, const Queen &target) const{
    return source.canJoin(target);
}

void Solver::move(Queen *source, Queen *target) {
    source->setExists(false);
    target->setPower(target->getPower()+ static_cast<unsigned short>(1));
    moves.push_back(Move(source,target));
    ignoreUselessNeighbours(source);
    ignoreUselessNeighbours(target);
    queenCount--;
}

/* Cannot be used in the solution, we are now solving a problem for k:=k-1 */
inline void Solver::ignore(Queen *queen) {
    move(queen,queen);
    target--;
}

void Solver::undoStep() {
    undoIgnores(); // ignored queens
    undoMove(); // the move itself
}

/* Ignoring a queens still counts as move but with equal source and target
 * ignoring is done after each move so we must first dispose of these reductions
 */
void Solver::undoIgnores() {
    while (!moves.empty() && std::get<0>(moves.back()) == std::get<1>(moves.back())) {
        undoIgnore();
    }
}

/* Takes a single move from stack and restores the state before it */
void Solver::undoMove() {
    Queen* source = std::get<0>(moves.back());
    Queen* target = std::get<1>(moves.back());
    moves.pop_back();
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
    queenCount++;
}

/* Takes a single move from stack restores the state before it - treats it as an ignore-move   */
void Solver::undoIgnore() {
    undoMove();
    target++;
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
        return lhs->getPower() > rhs->getPower();
    });
}
















