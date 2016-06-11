#include <algorithm>
#include "../include/Queen.h"

inline short Queen::getPower() const {
    return power;
}

inline void Queen::setPower(const short power) {
    Queen::power = std::min(power,MAX_QUEEN_POWER);
}



