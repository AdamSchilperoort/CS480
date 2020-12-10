#include "pcb.h"

#ifndef SIMULATOR
#define SIMULATOR

int simulate(struct simAction *actionsList, struct configValues *settings, struct logEvent *logList);

#endif

