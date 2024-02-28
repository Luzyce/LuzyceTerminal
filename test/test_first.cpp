#include <Arduino.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "terminal/error/IError.hpp"
#include "terminal/modules.hpp"
#include "terminal/networking/INetworking.hpp"
#include "terminal/scanner/IScanner.hpp"
#include "terminal/modules.cpp"


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

using ::testing::Return;

TEST(ModulesTest, InitCallsScanAndReturnsThree) {
  MockNetworking mockNet;
  MockScanner mockScan;
  MockError mockError;

  EXPECT_CALL(mockScan, scan())
      .WillOnce(Return(3));

  Modules modules(mockNet, mockScan, mockError);
  modules.init();
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