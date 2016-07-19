#include <algorithm>
#include <math.h>
#include <limits.h>
#include "../include/Queen.h"
#include "../include/Solver.h"

unsigned short Queen::powerFromExternal(const unsigned long long power) {
    return static_cast<unsigned short>(log2(power));
}

unsigned long long Queen::powerToExternal(const unsigned short power) {
    return static_cast<unsigned long long int> (pow(2, power));
}

bool Queen::isConnected(const Queen &other) const {
    for (auto connection : connections) {
        if (connection.second == &other) {
            return true;
        }
    }
    return false;
}

unsigned short Queen::viableConnectionCount() const {
    unsigned short count = 0;
    for (auto connection : connections) {
        if (canJoin(*connection.second)){
            count++;
        }
    }
    return count;
}

const Direction Queen::directionTo(const Queen *other) const{
    for (auto entry : connections ) {
        if (entry.second == other) {
            return entry.first;
        }
    }
    std::__throw_logic_error("Connection improperly setup. One-way.");
}

/*
 *  If at current time there is no neighbour with lower power - all have higher power,
 *  then we can assume that the current queen won't be useful for the solution
 *  because no chain of reduction can pass through it,
 *  therefore it is useless
 */
bool Queen::useless() {
    for (auto connection: connections) {
        Queen* other = Queen::findFutureJoinableQueen(this, connection.second);
        if (other) {
            return false;
        }
    }
    return true;
}

/* Find a queen in line with which there is any possiblity of joining */
Queen *Queen::findActiveQueen(Queen *source, Queen *target) {
    if ((target->doesExist() && target->getPower() <= source->getPower())) {
        return target;
    }
    Direction direction = source->directionTo(target);
    Queen* current = target;
    while(!current->doesExist() && current->isConnected(direction)) {
        current = current->getConnection(direction);
    }
    if (current->doesExist() && current->getPower() <= source->getPower()) {
        return current;
    } else {
        return nullptr;
    }
}

/* Find an active(existing) queen */
Queen *Queen::findFutureJoinableQueen(Queen *source, Queen *target) {
    if (target->doesExist()) {
        return target;
    }
    bool found = false;
    Direction direction = source->directionTo(target);
    Queen* current = target;
    while(!current->doesExist() && current->isConnected(direction)) {
        current = current->getConnection(direction);
        found = true;
    }
    if (found) {
        return current;
    } else {
        return nullptr;
    }
}

short Queen::rating() const {
        short connection = connectionCount()*CONNECTION_COUNT_WEIGHT;
        short powerCoefficient = power*POWER_WEIGHT;
        return connection+powerCoefficient;
}




















