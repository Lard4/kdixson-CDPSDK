#include "Packet.h"
#include "log.h"
#include "Utils.h"
#include "../crypto/duckcrypto.h"
#include "BloomFilter.h"

#include <vector>
#include <CDPError.h>
#include <zlib.h>

bool Packet::prepareForRelaying(BloomFilter *filter, std::vector<byte> dataBuffer) {

   this->reset();

   loginfo("prepareForRelaying: START");
   loginfo("prepareForRelaying: Packet is built. Checking for relay...");

   //TODO: Add bloom filter empty when full
   //TODO: Add 2nd bloom filter
   //TODO: Calculate false positive chance
   //TODO: Add backwards compatibility

   // Query the existence of strings
   bool alreadySeen = filter->bloom_check(&dataBuffer[MUID_POS], MUID_LENGTH);
   if (alreadySeen) {
      logdebug("handleReceivedPacket: Packet already seen. No relay.");
      return false;
   } else {
      filter->bloom_add(&dataBuffer[MUID_POS], MUID_LENGTH);
      logdebug("handleReceivedPacket: Relaying packet: " + duckutils::convertToHex(&dataBuffer[MUID_POS], MUID_LENGTH));
   }

   // update the rx packet internal byte buffer
   buffer.assign(dataBuffer.begin(), dataBuffer.end());
   int hops = buffer[HOP_COUNT_POS]++;
   loginfo("prepareForRelaying: hops count: %d", hops);
   return true;
}

void Packet::getUniqueMessageId(BloomFilter * filter, byte message_id[MUID_LENGTH]) {
   bool getNewUnique = true;
   while (getNewUnique) {
      duckutils::getRandomBytes(MUID_LENGTH, message_id);
      getNewUnique = filter->bloom_check(message_id, MUID_LENGTH);
      loginfo("prepareForSending: new MUID -> " + duckutils::convertToHex(message_id, MUID_LENGTH));
   }
}

int Packet::prepareForSending(BloomFilter *filter,
                              std::vector<byte> targetDevice,
                              byte duckType,
                              byte topic,
                              std::vector<byte> app_data) {
   std::vector<uint8_t> encryptedData;
   uint8_t app_data_length = app_data.size();

   this->reset();

   if (app_data_length > MAX_DATA_LENGTH) {
      return PACKET_ERR_SIZE_INVALID;
   }

   loginfo("prepareForSending: DATA LENGTH: %d TOPIC: %d", app_data_length, topic);

   byte message_id[MUID_LENGTH];
   getUniqueMessageId(filter, message_id);

   byte crc_bytes[DATA_CRC_LENGTH];
   uint32_t value;
   if (duckcrypto::getState()) {
      encryptedData.resize(app_data.size());
      duckcrypto::encryptData(app_data.data(), encryptedData.data(), app_data.size());
      value = crc32(0, encryptedData.data(), encryptedData.size());
   } else {
      value = crc32(0, app_data.data(), app_data.size());
   }

   crc_bytes[0] = (value >> 24) & 0xFF;
   crc_bytes[1] = (value >> 16) & 0xFF;
   crc_bytes[2] = (value >> 8) & 0xFF;
   crc_bytes[3] = value & 0xFF;

   // ----- insert packet header  -----
   // source device uid
   buffer.insert(buffer.end(), duid.begin(), duid.end());
   logdebug("SDuid:     " + duckutils::convertToHex(duid.data(), duid.size()));

   // destination device uid
   buffer.insert(buffer.end(), targetDevice.begin(), targetDevice.end());
   logdebug("DDuid:     " + duckutils::convertToHex(targetDevice.data(), targetDevice.size()));

   // message uid
   buffer.insert(buffer.end(), &message_id[0], &message_id[MUID_LENGTH]);
   logdebug("Muid:      " + duckutils::convertToHex(buffer.data(), buffer.size()));

   // topic
   buffer.insert(buffer.end(), topic);
   logdebug("Topic:     " + duckutils::convertToHex(buffer.data(), buffer.size()));

   // duckType
   buffer.insert(buffer.end(), duckType);
   logdebug("duck type: " + duckutils::convertToHex(buffer.data(), buffer.size()));

   // hop count
   buffer.insert(buffer.end(), 0x00);
   logdebug("hop count: " + duckutils::convertToHex(buffer.data(), buffer.size()));

   // data crc
   buffer.insert(buffer.end(), &crc_bytes[0], &crc_bytes[DATA_CRC_LENGTH]);
   logdebug("Data CRC:  " + duckutils::convertToHex(buffer.data(), buffer.size()));

   // ----- insert data -----
   if(duckcrypto::getState()) {

      buffer.insert(buffer.end(), encryptedData.begin(), encryptedData.end());
      logdebug("Encrypted Data:      " + duckutils::convertToHex(buffer.data(), buffer.size()));

   } else {
      buffer.insert(buffer.end(), app_data.begin(), app_data.end());
      logdebug("Data:      " + duckutils::convertToHex(buffer.data(), buffer.size()));
   }

   // ----- insert path -----
   // buffer.insert(buffer.end(), duid.begin(), duid.end());
   // logdebug("Path:      " + duckutils::convertToHex(buffer.data(), buffer.size()));

   logdebug("Built packet: " +
            duckutils::convertToHex(buffer.data(), buffer.size()));
   return ERR_NONE;
}