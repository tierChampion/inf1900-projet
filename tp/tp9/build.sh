#! /bin/bash

make clean -C ../lib
make -C ../lib
make clean
make
make install
