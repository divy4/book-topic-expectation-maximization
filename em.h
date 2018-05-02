#ifndef _EM_H_
#define _EM_H_

#include <iostream>

#include "constants.h"





// initialize EM data
void emInit();
// cleanup EM data
void emClose();

// main thread steps
void emMainEStep();
void emMainMStep();

// worker thread steps
void emWorkerEStep( const size_t& id );
void emWorkerMStep( const size_t& id );



#endif
