#ifndef ADPTO_DEBUGGING_H

//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
#include <sstream>
#include <iostream>
#define DEBUG(x) std::cerr << x << std::endl;
#else
#define DEBUG(x)
#endif

#define ADPTO_DEBUGGING_H
#endif //ADPTO_DEBUGGING_H
