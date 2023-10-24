#ifndef DEBUG_H
#define DEBUG_H

#include <string.h>
#include <iostream>

#include "comm.h"

#define DEBUG

#ifdef DEBUG
#define PRINT(x) display(x)

#else
#define PRINT(X)

#endif

void display(std::string x) {
}

#endif