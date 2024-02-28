#include "modules.hpp"

Modules::Modules(INetworking& net, IScanner& scan, IError& err)
    : net(net), scan(scan), err(err) {}

void Modules::init() {
    net.init();
    err.error(scan.scan());
}