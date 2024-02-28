#include "modules.hpp"

Modules::Modules(INetworking& net, IScanner& scan)
    : net(net), scan(scan) {}

void Modules::init() {
    net.init();
    scan.scan();
}