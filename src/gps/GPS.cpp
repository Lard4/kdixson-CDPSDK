#include "CDPInterface.h"
#include "GPS.h"

#define INDENT_SPACES "  "

GPS::GPS(CDPInterface* intf, uint8_t maxNmeaSentences) {
   this->intf = intf;
   this->intf->setupGPS();
   this->maxNmeaSentences = maxNmeaSentences;
   this->gpsData = {
         .rmcValid = false,
         .ggaValid = false,
   };
}

/**
 * Return a structure containing all of the data and only parse the raw one time.
 */
GPSReadings GPS::getAllReadings() {
   this->parse();
   return GPSReadings {
         .latitude = this->getLatitude(false),
         .longitude = this->getLongitude(false),
         .hours = this->getHours(false),
         .minutes = this->getMinutes(false),
         .seconds = this->getSeconds(false),
         .timeMillis = this->getTimeMillis(false),
   };
}

/**
 * Return Decimal latitude or -1 if invalid/insufficient data
 */
float GPS::getLatitude() {
   return this->getLatitude(true);
}

/**
 * Return Decimal longitude or -1 if invalid/insufficient data
 */
float GPS::getLongitude() {
   return this->getLongitude(true);
}

/**
 * Return UTC hours or -1 if invalid/insufficient data
 */
int GPS::getHours() {
   return this->getHours(true);
}

/**
 * Return UTC minutes or -1 if invalid/insufficient data
 */
int GPS::getMinutes() {
   return this->getMinutes(true);
}

/**
 * Return UTC seconds or -1 if invalid/insufficient data
 */
int GPS::getSeconds() {
   return this->getSeconds(true);
}

/**
 * Return current UTC epoch in milliseconds or -1 if invalid/insufficient data
 */
long GPS::getTimeMillis() {
   return this->getTimeMillis(true);
}

/**
 * Return Decimal latitude or -1 if invalid/insufficient data
 */
float GPS::getLatitude(bool parse) {
   if (parse) this->parse();
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return minmea_tocoord(&(gpsData.rmc).latitude);
   }
}

/**
 * Return Decimal longitude or -1 if invalid/insufficient data
 */
float GPS::getLongitude(bool parse) {
   if (parse) this->parse();
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return minmea_tocoord(&(gpsData.rmc).longitude);
   }
}

/**
 * Return UTC hours or -1 if invalid/insufficient data
 */
int GPS::getHours(bool parse) {
   if (parse) this->parse();
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return gpsData.rmc.time.hours;
   }
}

/**
 * Return UTC minutes or -1 if invalid/insufficient data
 */
int GPS::getMinutes(bool parse) {
   if (parse) this->parse();
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return gpsData.rmc.time.minutes;
   }
}

/**
 * Return UTC seconds or -1 if invalid/insufficient data
 */
int GPS::getSeconds(bool parse) {
   if (parse) this->parse();
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return gpsData.rmc.time.seconds;
   }
}

/**
 * Return current UTC epoch in milliseconds or -1 if invalid/insufficient data
 */
long GPS::getTimeMillis(bool parse) {
   if (parse) this->parse();
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      struct timespec time = {};
      if (-1 == minmea_gettime(&time, &(gpsData.rmc.date),&(gpsData.rmc.time))) {
         return -1;
      } else {
         return (time.tv_sec * 1000) + (time.tv_nsec / 1.0e6);
      }
   }
}

/**
 * Parses rawData into a GPSData structure, so data are easily extracted by other public functions.
 *
 * @param rawData the raw input buffer from the gps hardware.
 */
void GPS::parse() {
   // acquire a semaphore so the data doesn't ever change under our feet while parsing.
   this->intf->acquireSemaphore();

   std::vector<uint8_t> rawData = this->intf->getGPSBuffer();
   // make sure it's nul-terminated since we're about to work with char* strings.
   rawData.push_back(0);

   // convert from uint8_t to char
   char* end = reinterpret_cast<char*>(rawData.data());
   char* sentence;
   while (nullptr != (sentence = strsep(&end, "\r\n"))) {
      if (sentence[0] != '$') {
         // strsep does funky splitting with multi-char delimiters this limits noise
         continue;
      }

      switch (minmea_sentence_id(sentence, false)) {
         case MINMEA_SENTENCE_RMC: {
            if (minmea_parse_rmc(&(gpsData.rmc), sentence)) {
               gpsData.rmcValid = true;
               logdebug(
                   INDENT_SPACES
                   "$xxRMC time:%d:%d:%d fixed-point coordinates, speed scaled to three decimal places: (%d,%d) %d\n",
                   this->gps_data.rmc.time.hours,
                   this->gps_data.rmc.time.minutes,
                   this->gps_data.rmc.time.seconds,
                   minmea_rescale(&(this->gps_data.rmc).latitude, 1000),
                   minmea_rescale(&(this->gps_data.rmc).longitude, 1000),
                   minmea_rescale(&(this->gps_data.rmc).speed, 1000));
            } else {
               logwarn(INDENT_SPACES "$xxRMC sentence is not parsed\n");
            }
         } break;

         case MINMEA_SENTENCE_GGA: {
            if (minmea_parse_gga(&(gpsData.gga), sentence)) {
               gpsData.ggaValid = true;
               logdebug(INDENT_SPACES
                       "$xxGGA: sats used: %d fix quality: %d\n",
                       this->gps_data.gga.satellites_tracked,
                       this->gps_data.gga.fix_quality);
            } else {
               logwarn(INDENT_SPACES "$xxGGA sentence is not parsed\n");
            }
         } break;

         case MINMEA_SENTENCE_GSV:
         case MINMEA_SENTENCE_VTG:
         case MINMEA_SENTENCE_GLL:
            // ignore GSV, VTG, GLL
            break;

         case MINMEA_INVALID: {
            logwarn(INDENT_SPACES "$xxxxx sentence is not valid (%s)\n", sentence);
         } break;

         default: {
            loginfo(INDENT_SPACES "$xxxxx sentence is not parsed(%02x%02x)\n", sentence[0], sentence[1]);
         } break;
      }
   }

   // release the semaphore since we finished parsing.
   this->intf->releaseSemaphore();
}