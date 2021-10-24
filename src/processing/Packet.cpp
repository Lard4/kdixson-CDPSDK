#include "Packet.h"
#include "log.h"
#include "../util/Utils.h"
#include "../crypto/duckcrypto.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <CDPError.h>


bool Packet::prepareForRelaying(std::vector<byte> duid, std::vector<byte> dataBuffer) {

   bool relaying;

   int packet_length = dataBuffer.size();
   int path_pos = dataBuffer[PATH_OFFSET_POS];

   std::vector<byte> path_section;

   // extract path section from the Packet buffer
   path_section.assign(&dataBuffer[path_pos], &dataBuffer[packet_length]);

   this->reset();

   loginfo("prepareForRelaying: START");

   // update the rx Packet byte buffer
   buffer.assign(dataBuffer.begin(), dataBuffer.end());

   loginfo("prepareForRelaying: Packet is built. Checking for relay...");

   // when a Packet is relayed the given duid is added to the path section of the
   // Packet
   relaying = relay(duid, path_section);
   if (!relaying) {
      this->reset();
   }
   loginfo("prepareForRelaying: DONE. Relay = " + String(relaying));
   return relaying;
}

bool Packet::relay(std::vector<byte> duid, std::vector<byte> path_section) {

   int path_length = path_section.size();

   if (path_length == MAX_PATH_LENGTH) {
      logerror("ERROR Max hops reached. Cannot relay Packet.");
      return false;
   }

   // we don't have a contains() method but we can use indexOf()
   // if the result is > 0 then the substring was found
   // starting at the returned index value.
   std::string id = duckutils::convertToHex(duid.data(), duid.size());
   std::string path_string = duckutils::convertToHex(path_section.data(), path_length);
   if (path_string.find(id) >= 0) {
      loginfo("Packet already seen. ignore it.");
      return false;
   }
   buffer.insert(buffer.end(), duid.begin(), duid.end());
   buffer[HOP_COUNT_POS]++;
   return true;
}

int Packet::prepareForSending(std::vector<byte> targetDevice, byte duckType, byte topic, std::vector<byte> app_data) {

   std::vector<uint8_t> encryptedData;
   uint8_t app_data_length = app_data.size();

   this->reset();

   if (app_data_length > MAX_DATA_LENGTH) {
      return DUCKPACKET_ERR_SIZE_INVALID;
   }

   loginfo("prepareForSending: DATA LENGTH: %d, TOPIC: %d", app_data_length, topic);

   byte message_id[MUID_LENGTH];
   duckutils::getRandomBytes(MUID_LENGTH, message_id);

   byte crc_bytes[DATA_CRC_LENGTH];
   uint32_t value;
   // TODO: update the CRC32 library to return crc as a byte array
   if(duckcrypto::getState()) {
      encryptedData.resize(app_data.size());
      duckcrypto::encryptData(app_data.data(), encryptedData.data(), app_data.size());
      value = CRC32::calculate(encryptedData.data(), encryptedData.size());
   } else {
      value = CRC32::calculate(app_data.data(), app_data.size());
   }

   crc_bytes[0] = (value >> 24) & 0xFF;
   crc_bytes[1] = (value >> 16) & 0xFF;
   crc_bytes[2] = (value >> 8) & 0xFF;
   crc_bytes[3] = value & 0xFF;

   // ----- insert Packet header  -----
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

   // path offset
   byte offset = HEADER_LENGTH + app_data_length;
   buffer.insert(buffer.end(), offset);
   logdebug("Offset:    " + duckutils::convertToHex(buffer.data(), buffer.size()));

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
   buffer.insert(buffer.end(), duid.begin(), duid.end());
   logdebug("Path:      " + duckutils::convertToHex(buffer.data(), buffer.size()));

   logdebug("Built Packet: " + duckutils::convertToHex(buffer.data(), buffer.size()));
   return DUCK_ERR_NONE;
}
