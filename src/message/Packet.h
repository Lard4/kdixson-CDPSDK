#ifndef DUCKPACKET_H_
#define DUCKPACKET_H_

#include "Utils.h"
#include "BloomFilter.h"
#include "CDPPacket.h"

#include <vector>

static std::vector<byte> ZERO_DUID = {0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00};

static std::vector<byte> BROADCAST_DUID = {0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFF, 0xFF};

/**
 * @brief Encapsulate the protocol packet attributes and operations
 *
 */
class Packet {

public:

   /**
    * @brief Construct a new Packet object.
    *
    */
   Packet() {}
   /**
    * @brief Construct a new Packet object.
    *
    * @param duid a duck device unique id
    */
   Packet(std::vector<byte> duid) { this->duid = duid; }

   ~Packet() {}
   /**
    * @brief Set device Id.
    *
    * @param duid a duck device unique id
    */
   void setDeviceId(std::vector<byte> duid) { this->duid = duid; }

   /**
    * @brief Build a packet from the given topic and provided byte buffer.
    *
    * @param targetDevice the target device DUID to receive the message
    * @param topic a message topic
    * @param app_data a byte buffer that contains the packet data section
    * @returns DUCK_ERR_NONE if the operation was successful, otherwise an error code.
    */
   int prepareForSending(BloomFilter *filter, const std::vector<byte> targetDevice,
                         byte duckType, byte topic, std::vector<byte> app_data);

   /**
    * @brief Update a received packet if it needs to be relayed in the mesh.
    *
    * @param filter The bloom filter describing what packets have already been seen
    * @param dataBuffer buffer containing the packet data
    * @returns true if the packet needs to be relayed
    * @returns false if the packet does not need to be replayed
    */
   bool prepareForRelaying(BloomFilter *filter, std::vector<byte> dataBuffer);

   /**
    * @brief Get the Cdp Packet byte vector.
    *
    * @returns a vector of bytes representing the cdp packet
    */
   std::vector<byte> getBuffer() { return buffer;}

   /**
    * @brief Resets the packet byte buffer.
    *
    */
   void reset() {
      std::vector<byte>().swap(buffer);
   }

   byte getTopic() { return buffer[TOPIC_POS]; }


private:
   std::vector<byte> duid;
   std::vector<byte> buffer;

   static void getUniqueMessageId(BloomFilter * filter, byte message_id[MUID_LENGTH]);

};

#endif