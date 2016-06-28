#include <algorithm>
#include <math.h>
#include "../include/Queen.h"

void Queen::setPower(const unsigned short power) {
    Queen::power = std::max(MIN_QUEEN_POWER,std::min(power,MAX_QUEEN_POWER));
}

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


bool Queen::canJoin(const Queen &other) const {
    return exists && other.exists && power == other.power;
}

unsigned short Queen::connectionCount() const {
    return static_cast<unsigned short>(connections.size());
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













