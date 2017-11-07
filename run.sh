#!/bin/bash

set -e

# cd to repo root
cd $(git rev-parse --show-toplevel)

mkdir -p out/

g++ \
    ant.cpp \
    cell.cpp \
    controller.cpp \
    creator.cpp \
    doodlebug.cpp \
    grid.cpp \
    main.cpp \
    organism.cpp \
    organismimpl.cpp \
    -o out/doodlebug

out/doodlebug
