#! /bin/bash

make clean -C ../lib
make debug -C ../lib
make clean
make debug
make install