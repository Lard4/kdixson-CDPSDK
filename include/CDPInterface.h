#ifndef CDPSDK_CDPINTERFACE_H
#define CDPSDK_CDPINTERFACE_H

#include <string>
#include <vector>
#include <iostream>

class CDPInterface {
public:
   virtual ~CDPInterface() = default;

   /* gps interface */
   virtual std::vector<uint8_t> getGPSBuffer() = 0;
   virtual void setupGPS() = 0;

   /* LoRa interface */
   virtual void setupLora() = 0;
   virtual std::vector<uint8_t> getLoraBuffer() = 0;

   /* Hardware semaphore interface */
   virtual void acquireSemaphore() = 0;
   virtual void releaseSemaphore() = 0;

   virtual void /*TODO: make this return a type*/ getTimer() = 0;
};

#endif //CDPSDK_CDPINTERFACE_H
