#! /bin/bash

make clean -C ../lib
make -C ../lib
make clean -C exec
make -C exec
make install -C exec
