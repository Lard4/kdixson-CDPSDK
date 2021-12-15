#include "CDPError.h"
#include "CDPInterface.h"

class MockInterfaceBase: public CDPInterface {
public:
   MockInterfaceBase(LoraInterface *loraInterface, GPSInterface *gps) : CDPInterface(loraInterface, gps) {}

   class MockLoraInterfaceBase: public LoraInterface {
   public:
      int setup() override {
         return ERR_NONE;
      }

      int setFrequency(float band) override {
         return ERR_NONE;
      }

      int setBandwidth(float bandwidth) override {
         return ERR_NONE;
      }

      int setSpreadingFactor(uint8_t spreadingFactor) override {
         return ERR_NONE;
      }

      int setOutputPower(int8_t txPower) override {
         return ERR_NONE;
      }

      int setGain(uint8_t gain) override {
         return ERR_NONE;
      }

      int setCallbackFunction(void (*callback)()) override {
         return ERR_NONE;
      }

      int setSyncWord(uint8_t word) override {
         return ERR_NONE;
      }

      // getters
      int readData(uint8_t* buffer, size_t len) override {
         return ERR_NONE;
      }

      int getPacketLength() override {
         return ERR_NONE;
      }

      int getRSSI() override {
         return ERR_NONE;
      }

      // mode control
      int startReceive() override {
         return ERR_NONE;
      }

      int standby() override {
         return ERR_NONE;
      }

      int transmit(uint8_t* buffer, size_t len) override {
         return ERR_NONE;
      }

      int sleep() override {
         return ERR_NONE;
      }
   };

   class MockGPSInterfaceBase: public GPSInterface {
   public:
      int setup() override {
         return ERR_NONE;
      }

      std::vector<uint8_t> getBuffer() override {
         return std::vector<uint8_t>();
      }
   };

   void getTimer() override {}
   void acquireSemaphore() override {}
   void releaseSemaphore() override {}
   void restart() override {}
   void setupSerial(int buadRate) override {}
};