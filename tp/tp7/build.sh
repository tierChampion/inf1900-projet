#! /bin/bash

make -C lib
make -C exec
make install -C exec
