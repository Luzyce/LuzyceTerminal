#!/bin/bash

if [ -d "LuzyceTerminal" ]; then
    rm -rf LuzyceTerminal
fi

git clone https://github.com/Luzyce/LuzyceTerminal.git -b $BRANCH
cd LuzyceTerminal
pio test -e utest