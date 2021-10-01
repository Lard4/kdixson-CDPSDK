#ifndef CDPSDK_GPS_H
#define CDPSDK_GPS_H

#include <semaphore>

#include "minmea.h"


typedef struct {
   struct minmea_sentence_rmc rmc;
   bool rmcValid;
   struct minmea_sentence_gga gga;
   bool ggaValid;
} GPSData;


class GPS {
public:
   GPS(uint8_t maxNmeaSentences);
   float getLatitude();
   float getLongitude();
   int getHours();
   int getMinutes();
   int getSeconds();
   long getTimeMillis();
   void parse(char* rawData);

private:
   // structure to store GPS data
   GPSData gpsData;
   // total number of NMEA sentences that the GPS hardware can compute
   uint8_t maxNmeaSentences;
};


#endif //CDPSDK_GPS_H
