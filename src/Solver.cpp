#include <algorithm>
#include "../include/Solver.h"
#include "../include/Debugging.h"
#include "../include/Tools.h"

bool Solver::possible(unsigned int solutionRequirement) {
    target = kernelize(solutionRequirement);
    queenCount = countQueens();
    DEBUG("Solver: We have: " << queenCount << " queens with a target of " << target << std::endl );
    outlineQueens();
    DEBUG("After kernelization:");
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
                        undo();
                    }
                }
            }
        }
    }
    return false;
}

unsigned int Solver::kernelize(unsigned int target) {
    unsigned int removed = 0;

    for (auto entry : *board.getQueens()) {
        if (entry.second->getConnections()->empty()) {
            entry.second->setExists(false);
            removed++;
        }
    }
    return target-removed;
}

bool Solver::moveValid(const Queen &source, const Queen &target) const{
    return source.canJoin(target);
}

void Solver::move(Queen *source, Queen *target) {
    source->setExists(false);
    target->setPower(target->getPower()+ static_cast<unsigned short>(1));
    moves.push_back(new Move(source,target));
    queenCount--;
}

void Solver::undo() {
    Queen* source = std::get<0>(*moves.back());
    Queen* target = std::get<1>(*moves.back());
    delete moves.back();
    moves.pop_back();
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
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
        return lhs->getPower() < rhs->getPower();
    });
}















