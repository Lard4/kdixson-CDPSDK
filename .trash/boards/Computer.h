#ifndef CDPSDK_COMPUTER_H
#define CDPSDK_COMPUTER_H

#include <vector>
#include <random>

#include "CDPInterface.h"

class Computer: public CDPInterface {
public:
   // overrides
   void setupGPS() override;
   std::vector<uint8_t> getGPSBuffer() override;
   void setupLora() override;
   std::vector<uint8_t> getLoraBuffer() override;
   void acquireSemaphore() override;
   void releaseSemaphore() override;
   void /*TODO: make this return a type*/ getTimer() override;

   // supporting functions
   [[noreturn]] void mockDataThread();
   static std::vector<uint8_t> getMockGPSValues();

private:
   std::vector<uint8_t> rawGPSData;
};


#endif //CDPSDK_COMPUTER_H
