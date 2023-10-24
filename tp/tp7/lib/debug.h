#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>

#include "comm.h"

#define DEBUG

#ifdef DEBUG

#else

#endif

void display(std::string x) {
    std::cout << x << std::endl;
}

#endif