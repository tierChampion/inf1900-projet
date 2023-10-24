#ifndef DEBUG_H
#define DEBUG_H

#include <string.h>
#include <stdio.h>

#include "comm.h"

#ifdef DEBUG
#define PRINT(x) display(x)

#else
#define PRINT(X)

#endif

void display(uint8_t x);

#endif