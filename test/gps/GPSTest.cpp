#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GPS.h"
#include "../MockInterfaceBase.h"

using namespace std;

/**
 * Since we aren't testing against actual hardware, we create a one here. While we're here, we make it a GoogleTest
 * "Mock" class so we can reflect and see that we called the appropriate hardware accessor methods with the right
 * paramters, the right amount of times, etc.
 */
class MockInterface: public MockInterfaceBase {
public:
   MockInterface(LoraInterface *loraInterface, GPSInterface *gps) : MockInterfaceBase(loraInterface, gps) {}

   MOCK_METHOD(void, acquireSemaphore, (), (override));
   MOCK_METHOD(void, releaseSemaphore, (), (override));

   class MockGPSInterface: public MockGPSInterfaceBase {
   public:
      MOCK_METHOD(int, setup, (), (override));

      // return an RMC sentence so we can test RMC parsing
      std::vector<uint8_t> getBuffer() override {
         string mockSentence = "$GPRMC,210230,A,3855.4487,N,09446.0071,W,0.0,076.2,130495,003.8,E*69";
         vector<uint8_t> retVal(mockSentence.begin(), mockSentence.end());
         return retVal;
      }
   };
};

TEST(GPSTest, ParseRMC) {
   // instantiate a concrete implementation of the CDP Interface. the shared_ptr helps GTest work cleanly.
   auto gps = make_shared<MockInterface::MockGPSInterface>();
   auto lora = make_shared<MockInterface::MockLoraInterfaceBase>();
   auto intf = make_shared<MockInterface>(lora.get(), gps.get());

   // EXPECT_CALL goes *before* the expected calls
   EXPECT_CALL(*gps, setup())
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

TEST(GPSTest, GetAllReadings) {
   auto gps = make_shared<MockInterface::MockGPSInterface>();
   auto lora = make_shared<MockInterface::MockLoraInterfaceBase>();
   auto intf = make_shared<MockInterface>(lora.get(), gps.get());

   EXPECT_CALL(*gps, setup())
         .Times(1);
   EXPECT_CALL(*intf, acquireSemaphore())
         .Times(1); // once because getAllReadings should only parse one time
   EXPECT_CALL(*intf, releaseSemaphore())
         .Times(1); // once because getAllReadings should only parse one time

   GPS gpsInst = GPS(intf.get(), 1);

   GPSReadings expectedReadings = {
         .latitude = 38.924145,
         .longitude = -94.766785,
         .hours = 21,
         .minutes = 2,
         .seconds = 30,
         .timeMillis = 797835750000,
   };
   GPSReadings actualReadings = gpsInst.getAllReadings();

   EXPECT_FLOAT_EQ(actualReadings.latitude, expectedReadings.latitude);
   EXPECT_FLOAT_EQ(actualReadings.longitude, expectedReadings.longitude);
   EXPECT_EQ(actualReadings.timeMillis, expectedReadings.timeMillis);
   EXPECT_EQ(actualReadings.seconds, expectedReadings.seconds);
   EXPECT_EQ(actualReadings.minutes, expectedReadings.minutes);
   EXPECT_EQ(actualReadings.hours, expectedReadings.hours);
}
