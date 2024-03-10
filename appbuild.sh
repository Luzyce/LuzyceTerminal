#!/bin/bash

curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | tee /etc/udev/rules.d/99-platformio-udev.rules

if [ -d "LuzyceTerminal" ]; then
    rm -rf LuzyceTerminal
fi

git clone https://github.com/Luzyce/LuzyceTerminal.git -b $BRANCH
cd LuzyceTerminal

sleep 10

pio test -e utest