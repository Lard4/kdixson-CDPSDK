#include <log.h>
#include "GPS.h"
#include "../../CDPInterface.h"

#define INDENT_SPACES "  "

GPS::GPS(uint8_t maxNmeaSentences) {
   // TODO: setup UART
   this->maxNmeaSentences = maxNmeaSentences;
   this->gpsData = {
         .rmcValid = false,
         .ggaValid = false,
   };
}

/**
 * Return Decimal latitude or -1 if invalid/insufficient data
 */
float GPS::getLatitude() {
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return minmea_tocoord(&(gpsData.rmc).latitude);
   }
}

/**
 * Return Decimal longitude or -1 if invalid/insufficient data
 */
float GPS::getLongitude() {
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return minmea_tocoord(&(gpsData.rmc).longitude);
   }
}

/**
 * Return UTC hours or -1 if invalid/insufficient data
 */
int GPS::getHours() {
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return gpsData.rmc.time.hours;
   }
}

/**
 * Return UTC minutes or -1 if invalid/insufficient data
 */
int GPS::getMinutes() {
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return gpsData.rmc.time.minutes;
   }
}

/**
 * Return UTC seconds or -1 if invalid/insufficient data
 */
int GPS::getSeconds() {
   if (!gpsData.rmcValid) {
      return -1;
   } else {
      return gpsData.rmc.time.seconds;
   }
}

/**
 * Return current UTC epoch in milliseconds or -1 if invalid/insufficient data
 */
long GPS::getTimeMillis() {
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
 * @param rawData the raw input buffer from the GPS hardware.
 */
void GPS::parse(char* rawData) {
   char* sentence = rawData;
   char* end = rawData;
   while (nullptr != (sentence = strsep(&end, "\r\n"))) {
      if (sentence[0] != '$') {
         // strsep does funky splitting with multi-char delimeters this limits noise
         continue;
      }

      switch (minmea_sentence_id(sentence, false)) {
         case MINMEA_SENTENCE_RMC: {
            if (minmea_parse_rmc(&(gpsData.rmc), sentence)) {
               gpsData.rmcValid = true;
               logdebug(
                   INDENT_SPACES
                   "$xxRMC time:%d:%d:%d fixed-point coordinates, "
                   "speed "
                   "scaled to three decimal places: (%d,%d) %d\n",
                   this->gps_data.rmc.time.hours,
                   this->gps_data.rmc.time.minutes,
                   this->gps_data.rmc.time.seconds,
                   minmea_rescale(&(this->gps_data.rmc).latitude,
                                   1000),
                   minmea_rescale(&(this->gps_data.rmc).longitude,
                                   1000),
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
            // ignore
            break;

         case MINMEA_INVALID: {
            loginfo(INDENT_SPACES "$xxxxx sentence is not valid (%02x%02x)\n", sentence[0], sentence[1]);
         } break;

         default: {
            loginfo(INDENT_SPACES "$xxxxx sentence is not parsed(%02x%02x)\n", sentence[0], sentence[1]);
         } break;
      }
   }
}