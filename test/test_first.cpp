#include <Arduino.h>
#include <gtest/gtest.h>

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