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
//    for (auto entry : *board.getQueens()) {
//        if (entry.second->getConnections()->empty()) {
//            board.getQueens()->erase(entry.first);
//            removed++;
//        }
//    }
    return target-removed;
}

bool Solver::moveValid(Queen &source, Queen &target) const{
    for (auto connection : *source.getConnections()) {
        if (&connection.second == &target) {
            return true;
        }
    }
    return false;
}

void Solver::move(const Queen &source, const Queen &target) {

}







