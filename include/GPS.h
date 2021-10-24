#ifndef CDPSDK_GPS_H
#define CDPSDK_GPS_H

#include <semaphore>

#include "minmea.h"
#include "CDPInterface.h"
#include "log.h"

typedef struct {
   float latitude;
   float longitude;
   int hours;
   int minutes;
   int seconds;
   long timeMillis;
} GPSReadings;

class GPS {
public:
   GPS(CDPInterface* intf, uint8_t maxNmeaSentences);

   GPSReadings getAllReadings();
   float getLatitude();
   float getLongitude();
   int getHours();
   int getMinutes();
   int getSeconds();
   long getTimeMillis();

private:
   typedef struct {
      struct minmea_sentence_rmc rmc;
      bool rmcValid;
      struct minmea_sentence_gga gga;
      bool ggaValid;
   } GPSData;

   float getLatitude(bool parse);
   float getLongitude(bool parse);
   int getHours(bool parse);
   int getMinutes(bool parse);
   int getSeconds(bool parse);
   long getTimeMillis(bool parse);

   // function to parse the raw data Packet from the hardware
   void parse();

   // reference to the interface to the hardware
   CDPInterface* intf;

   // structure to store gps data
   GPSData gpsData;

   // total number of NMEA sentences that the gps hardware can compute
   uint8_t maxNmeaSentences;
};


#endif //CDPSDK_GPS_H
