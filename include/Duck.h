#ifndef CDPSDK_DUCK_H
#define CDPSDK_DUCK_H


#include "GPS.h"

class Duck {
public:
   // constructor and destructor
   Duck();

   // gps module
   GPS* startGPS(uint8_t maxNmeaSentences);
   void stopGPS();
   GPS* getGPS();

private:
   GPS* gps;
   CDPInterface* interface;
};


#endif //CDPSDK_DUCK_H
