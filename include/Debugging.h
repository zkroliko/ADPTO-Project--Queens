//
// Created by zbkro on 14.06.2016.
//

#ifndef ADPTO_DEBUGGING_H

#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
#define DEBUG(x) std::cerr << x << endl;
#else
#define DEBUG(x)
#endif

#define ADPTO_DEBUGGING_H
#endif //ADPTO_DEBUGGING_H
