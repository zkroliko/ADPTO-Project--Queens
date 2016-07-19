#include "../include/Actions.h"
#include "../include/Board.h"
#include<sstream>

/* Returns the number of reductions in this move */
short Move::apply(Board& board) {
    source->setExists(false);
    target->setPower(target->getPower()+ static_cast<unsigned short>(1));
    return 0;
}

/* Returns the number of reverted reductions in this move */
short Move::undo(Board& board) {
    for (auto reduction: reductions) {
        reduction->undo(board);
    }
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
    return static_cast<unsigned short>(reductions.size());
}

string Move::toString() {
    std::stringstream ss;
    ss << std::get<0>(source->getPosition()) << " " << std::get<1>(source->getPosition());
    ss << " ";
    ss << std::get<0>(target->getPosition()) << " " << std::get<1>(target->getPosition());
    return ss.str();
}

short Ignore::apply(Board& board) {
    if (queen->doesExist()) {
        queen->setExists(false);
    } else {
        std::__throw_logic_error("Cannot ignore already inactive queen");
    }
    return -1;
}

short Ignore::undo(Board& board) {
    queen->setExists(true);
    return 1;
}






