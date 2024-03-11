#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "terminal/interfaces/IConsole.hpp"
#include "terminal/interfaces/IKeypad.hpp"
#include "terminal/interfaces/ILcd.hpp"
#include "terminal/interfaces/IMcp.hpp"
#include "terminal/interfaces/INetworking.hpp"
#include "terminal/interfaces/INfc.hpp"
#include "terminal/interfaces/IQr.hpp"
#include "terminal/interfaces/IScanner.hpp"

#include "terminal/terminal.hpp"
#include "terminal/terminal.cpp"


class MockNetworking : public INetworking {
 public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(requestAnswer, request, (std::string subpage, std::string data), (override));
};

class MockScanner : public IScanner {
 public:
  MOCK_METHOD(int, scan, (), (override));
};

class MockConsole : public IConsole {
 public:
  MOCK_METHOD(void, print, (std::string data), (override));
};

class MockKeypad : public IKeypad {
 public:
  MOCK_METHOD(void, init, (), (override));
};

class MockLcd : public ILcd {
 public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, print, (uint8_t x, uint8_t y, std::string data), (override));
  MOCK_METHOD(void, clear, (), (override));
};

class MockMcp : public IMcp {
 public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, statusLed, (int led), (override));
};

class MockNfc : public INfc {
 public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(std::string, scan, (), (override));
};

class MockQr : public IQr {
 public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(std::string, scan, (), (override));
};

using ::testing::Return;

TEST(ModulesTest, InitCallsScanAndReturnsThree) {
  MockNetworking mockNet;
  MockScanner mockScan;
  MockConsole mockConsole;
  MockKeypad mockKeypad;
  MockLcd mockLcd;
  MockMcp mockMcp;
  MockNfc mockNfc;
  MockQr mockQr;

  EXPECT_CALL(mockScan, scan())
      .WillOnce(Return(3));

  Terminal terminal(mockNet, mockScan, mockKeypad, mockMcp, mockLcd, mockNfc, mockQr, mockConsole);
  terminal.init();
}

TEST(DummyTest, ShouldPass) { EXPECT_EQ(1, 1); }

#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
}

void loop()
{
	// Run tests
	if (RUN_ALL_TESTS())
	;

	// sleep 1 sec
	delay(1000);
}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}
#endif