#ifndef CDPSDK_CDPINTERFACE_H
#define CDPSDK_CDPINTERFACE_H

#include <string>
#include <vector>
#include <iostream>


class CDPInterface {
public:
   class LoraInterface {
   public:
      virtual int setup() = 0;

      // setters
      virtual int setFrequency(float band) = 0;
      virtual int setBandwidth(float bandwidth) = 0;
      virtual int setSpreadingFactor(uint8_t spreadingFactor) = 0;
      virtual int setOutputPower(int8_t txPower) = 0;
      virtual int setGain(uint8_t gain) = 0;
      virtual int setCallbackFunction(void (*callback)()) = 0;
      virtual int setSyncWord(uint8_t word) = 0;

      // getters
      virtual int readData(uint8_t* buffer, size_t len) = 0;
      virtual int getPacketLength() = 0;
      virtual int getRSSI() = 0;

      // mode control
      virtual int startReceive() = 0;
      virtual int standby() = 0;
      virtual int transmit(uint8_t* buffer, size_t len) = 0;
      virtual int sleep() = 0;
   };

   class GPSInterface {
   public:
      virtual int setup() = 0;

      // getters
      virtual std::vector<uint8_t> getBuffer() = 0;
   };

   explicit CDPInterface(
         LoraInterface* loraInterface,
         GPSInterface* gps
   ) {
      this->lora = loraInterface;
      this->gps = gps;
   }

   virtual ~CDPInterface() = default;

   /* Hardware semaphore interface */
   virtual void acquireSemaphore() = 0;
   virtual void releaseSemaphore() = 0;

   virtual void /*TODO: make this return a type*/ getTimer() = 0;

   virtual void restart() = 0;

   virtual void setupSerial(int baudRate) = 0;

   LoraInterface* lora;
   GPSInterface* gps;
};

#endif //CDPSDK_CDPINTERFACE_H
