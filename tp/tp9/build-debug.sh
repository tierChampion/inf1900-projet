#! /bin/bash

make clean -C lib
make debug -C lib
make clean -C exec
make debug -C exec
make install -C exec