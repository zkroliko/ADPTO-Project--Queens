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







