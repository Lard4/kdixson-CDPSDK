#include "Utils.h"
#include <string>
#include <random>

namespace duckutils {

   namespace {
      std::string cdpVersion = "2.10.2";
   }

   bool detectState = false;

   std::string getCDPVersion() {
      return cdpVersion;
   }

   bool getDetectState() { return detectState; }
   bool flipDetectState() {
      detectState = !detectState;
      return detectState;

   }

   void getRandomBytes(int length, byte* bytes) {
      //TODO: Random generator here isn't seeded properly
      std::default_random_engine generator;
      std::uniform_int_distribution<byte> distribution(0, UINT8_MAX);

      const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

      for (int i = 0; i < length; i++) {
         bytes[i] = digits[distribution(generator)];
      }
   }

   std::string createUuid(int length) {
      //TODO: Random generator here isn't seeded properly
      std::default_random_engine generator;
      std::uniform_int_distribution<byte> distribution(0, UINT8_MAX);

      std::string msg;

      for (int i = 0; i < length; i++) {
         byte randomValue = distribution(generator);
         if (randomValue < 26) {
            msg += char(randomValue + 'a');
         } else {
            msg += char((randomValue - 26) + '0');
         }
      }
      return msg;
   }

   std::string convertToHex(const byte* data, int size) {
      std::string buf;
      buf.reserve(size * 2); // 2 digit hex
      const char* cs = "0123456789ABCDEF";
      for (int i = 0; i < size; i++) {
         byte val = data[i];
         buf += cs[(val >> 4) & 0x0F];
         buf += cs[val & 0x0F];
      }
      return buf;
   }

   uint32_t toUnit32(const byte* data) {
      uint32_t value = 0;

      value |= data[0] << 24;
      value |= data[1] << 16;
      value |= data[2] << 8;
      value |= data[3];
      return value;
   }

} // namespace duckutils