#include "terminal.hpp"

Terminal::Terminal(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd, INfc& nfc, IQr& qr, IError& err)
    : net(net), scan(scan), key(key), mcp(mcp), lcd(lcd), nfc(nfc), qr(qr), err(err) {}


void Terminal::init() {
    net.init();
    err.error(scan.scan());
    key.init();
    mcp.init();
    lcd.init();
    nfc.init();
    qr.init();
}