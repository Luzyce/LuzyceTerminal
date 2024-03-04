#!/bin/bash

if [ -d "LuzyceTerminal" ]; then
    rm -rf LuzyceTerminal
fi

git clone https://github.com/BakcylProgramowania/paragon_back.git -b $BRANCH
cd LuzyceTerminal
pio test -e utest