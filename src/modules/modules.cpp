#include "modules.hpp"

void Modules::init() {
    net.init();

    Wire.begin();

    key.init();
}