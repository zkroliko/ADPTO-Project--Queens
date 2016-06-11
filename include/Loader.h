#ifndef ADPTO_LOADER_H
#define ADPTO_LOADER_H
#include "Board.h"

class Loader {
    public:
        static Board* load(const short size, const char* input[]);
};


#endif //ADPTO_LOADER_H
