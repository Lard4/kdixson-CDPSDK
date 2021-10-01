#include <gtest/gtest.h>
#include "../../src/GPS/GPS.h"

using namespace std;

// TEST has two parameters: the test case name and the test name.
TEST(GPSTest, Example) {
   // Expect two strings not to be equal.
   EXPECT_STRNE("hello", "world");
   // Expect equality.
   EXPECT_EQ(7 * 6, 42);
}

TEST(GPSTest, ParseRMC) {
   GPS gpsInst = GPS(1);

   // pretend like we just got data from the GPS module
   string ggaSentence = "$GPRMC,210230,A,3855.4487,N,09446.0071,W,0.0,076.2,130495,003.8,E*69";
   gpsInst.parse((char*) ggaSentence.c_str());

   EXPECT_FLOAT_EQ(gpsInst.getLatitude(), 38.924145);
   EXPECT_FLOAT_EQ(gpsInst.getLongitude(), -94.766785);

   EXPECT_EQ(gpsInst.getTimeMillis(), 797835750000);
   EXPECT_EQ(gpsInst.getSeconds(), 30);
   EXPECT_EQ(gpsInst.getMinutes(), 2);
   EXPECT_EQ(gpsInst.getHours(), 21);
}
