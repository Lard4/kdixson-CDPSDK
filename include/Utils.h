#ifndef CDPSDK_UTILS_H
#define CDPSDK_UTILS_H

#include "CDPError.h"

#include <string>
#include <vector>

#define UUID_MAX_LEN 8


typedef uint8_t byte;

namespace duckutils {

   extern bool detectState;

   std::string getCDPVersion();

/**
 * @brief Creates a byte array with random alpha numerical values.
 *
 * @param length the array length
 * @param bytes array of bytes of the specified length
 */
   void getRandomBytes(int length, byte* bytes);

/**
 * @brief Create a uuid string.
 *
 * @param length the length of the UUID (defaults to CDPCFG_UUID_LEN)
 * @returns A string representing a unique id.
 */
   std::string createUuid(int length = UUID_MAX_LEN);

/**
 * @brief Convert a byte array into a hex string.
 *
 * @param data a byte array to convert
 * @param size the size of the array
 * @returns A string representating the by array in hexadecimal.
 */
   std::string convertToHex(const byte* data, int size);

/**
 * @brief Convert a vector into an ASCII string.
 *
 * @param vec A vector to convert
 * @returns A String representing the byte array in ASCII.
 *
 */
   template<typename T>
   std::string toString(const std::vector<T> & vec) {
      return std::string(vec.begin(), vec.end());
   }

/**
 * @brief Compare two vectors with regard to both size and contents.
 *
 * @param a The first vector
 * @param b The second vector
 * @returns True if a and b have the same size and contents, false if not.
 */
   template<typename T>
   bool isEqual(const std::vector<T> & a, const std::vector<T> & b) {
      if (a.size() != b.size()) {
         return false;
      }
      return std::equal(a.begin(), a.end(), b.begin());
   }

/**
 * @brief Toggle the duck Interrupt
 *
 * @param enable true to enable interrupt, false otherwise.
 */
   void setInterrupt(bool enable);

/**
 * @brief Convert a byte array to unsigned 32 bit integer.
 *
 * @param data byte array to convert
 * @returns a 32 bit unsigned integer.
 */
   uint32_t toUnit32(const byte* data);

   bool getDetectState();
   bool flipDetectState();

} // namespace duckutils

#endif //CDPSDK_UTILS_H
