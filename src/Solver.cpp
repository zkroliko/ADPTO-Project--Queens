#include "../include/Solver.h"
#include "../include/Debugging.h"

bool Solver::possible(unsigned int target) {
    target = kernelize(target);
    DEBUG("After kernelization:");
    std::cerr << board.toString();
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

void Solver::move(Queen &source, Queen &target) {
    source.setExists(false);
    target.setPower(target.getPower()+ static_cast<unsigned short>(1));
    moves.push_back(Move(&source,&target));
}

void Solver::undo() {
    Queen* source = std::get<0>(moves.back());
    Queen* target = std::get<0>(moves.back());
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
}









