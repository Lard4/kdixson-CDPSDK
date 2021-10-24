#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GPS.h"

using namespace std;

/**
 * Since we aren't testing against actual hardware, we create a one here. While we're here, we make it a GoogleTest
 * "Mock" class so we can reflect and see that we called the appropriate hardware accessor methods with the right
 * paramters, the right amount of times, etc.
 */
class MockInterface: public CDPInterface {
public:
   MOCK_METHOD(void, setupGPS, (), (override));
   MOCK_METHOD(void, acquireSemaphore, (), (override));
   MOCK_METHOD(void, releaseSemaphore, (), (override));

   // return an RMC sentence so we can test RMC parsing
   std::vector<uint8_t> getGPSBuffer() override {
      string mockSentence = "$GPRMC,210230,A,3855.4487,N,09446.0071,W,0.0,076.2,130495,003.8,E*69";

      vector<uint8_t> retVal(mockSentence.begin(), mockSentence.end());

      return retVal;
   }

   // unused, but we need to write this to fully implement the CDPInterface
   void setupLora() override {}

   // unused, but we need to write this to fully implement the CDPInterface
   std::vector<uint8_t> getLoraBuffer() override {
      return vector<uint8_t>();
   }

   // unused, but we need to write this to fully implement the CDPInterface
   void getTimer() override {}
};

TEST(GPSTest, ParseRMC) {
   // instantiate a concrete implementation of the CDP Interface. the shared_ptr helps GTest work cleanly.
   auto intf = make_shared<MockInterface>();

   // EXPECT_CALL goes *before* the expected calls
   EXPECT_CALL(*intf, setupGPS())
         .Times(1);
   EXPECT_CALL(*intf, acquireSemaphore())
         .Times(6); // one for each `get` call
   EXPECT_CALL(*intf, releaseSemaphore())
         .Times(6); // one for each `get` call

   // create our gps instance, like normal
   GPS gpsInst = GPS(intf.get(), 1);

   // test the getters in our gps instance
   EXPECT_FLOAT_EQ(gpsInst.getLatitude(), 38.924145);
   EXPECT_FLOAT_EQ(gpsInst.getLongitude(), -94.766785);
   EXPECT_EQ(gpsInst.getTimeMillis(), 797835750000);
   EXPECT_EQ(gpsInst.getSeconds(), 30);
   EXPECT_EQ(gpsInst.getMinutes(), 2);
   EXPECT_EQ(gpsInst.getHours(), 21);

   // that's it!
}
