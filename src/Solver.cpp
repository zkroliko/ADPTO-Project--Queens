#include <algorithm>
#include "../include/Solver.h"
#include "../include/Debugging.h"
#include "../include/Tools.h"

bool Solver::possible(unsigned int target) {
    target = kernelize(target);
    queenCount = countQueens();
    DEBUG("Solver: We have: " << queenCount << " queens with a target of " << target << std::endl );
    outlineQueens();
    check();
    undo();
    undo();
    DEBUG("After kernelization:");
    std::cerr << board.toString();
    return true;
}

bool Solver::check() {
    Queen* source = board.get(0,0);
    Queen* target = board.get(0,1);
    move(source,target);
    source = board.get(1,0);
    target = board.get(1,1);
    move(source,target);
    return false;
}

unsigned int Solver::kernelize(unsigned int target) {
    unsigned int removed = 0;

    for (auto entry : *board.getQueens()) {
        if (entry.second->getConnections()->empty()) {
            entry.second->setExists(false);
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
}

void Solver::undo() {
    Queen* source = std::get<0>(*moves.back());
    Queen* target = std::get<1>(*moves.back());
    moves.pop_back();
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
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
    std::copy_if(retrieved.begin(), retrieved.end(),
            leftQueens.begin(),
            [&](const Queen* queen) { return queen->doesExist();});
}













