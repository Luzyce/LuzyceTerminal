#include <Arduino.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "terminal/interfaces/IError.hpp"
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
};

class MockScanner : public IScanner {
 public:
  MOCK_METHOD(int, scan, (), (override));
};

class MockError : public IError {
 public:
  MOCK_METHOD(void, error, (int code), (override));
};

class MockKeypad : public IKeypad {
 public:
  MOCK_METHOD(void, init, (), (override));
};

class MockLcd : public ILcd {
 public:
  MOCK_METHOD(void, init, (), (override));
};

class MockMcp : public IMcp {
 public:
  MOCK_METHOD(void, init, (), (override));
};

class MockNfc : public INfc {
 public:
  MOCK_METHOD(void, init, (), (override));
};

class MockQr : public IQr {
 public:
  MOCK_METHOD(void, init, (), (override));
};

using ::testing::Return;

TEST(ModulesTest, InitCallsScanAndReturnsThree) {
  MockNetworking mockNet;
  MockScanner mockScan;
  MockError mockError;
  MockKeypad mockKeypad;
  MockLcd mockLcd;
  MockMcp mockMcp;
  MockNfc mockNfc;
  MockQr mockQr;

  EXPECT_CALL(mockScan, scan())
      .WillOnce(Return(3));

  Terminal terminal(mockNet, mockScan, mockKeypad, mockMcp, mockLcd, mockNfc, mockQr, mockError);
  terminal.init();
}

TEST(DummyTest, ShouldPass) { EXPECT_EQ(1, 1); }

void setup() {
  Serial.begin(115200);

  ::testing::InitGoogleTest();
}

void loop() {
  if (RUN_ALL_TESTS())
    ;

  delay(1000);
}