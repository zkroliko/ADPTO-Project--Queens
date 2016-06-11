#include <algorithm>
#include "../include/Queen.h"

inline unsigned short Queen::getPower() const {
    return power;
}

inline void Queen::setPower(const unsigned short power) {
    Queen::power = std::min(power,MAX_QUEEN_POWER);
}



