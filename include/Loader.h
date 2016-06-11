#ifndef ADPTO_LOADER_H
#define ADPTO_LOADER_H
#include "Board.h"

class Loader {
    public:
        static Board* load(const unsigned short size, const std::vector<std::string> lines);
};


#endif //ADPTO_LOADER_H
