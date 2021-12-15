#include "Duck.h"
#include "../crypto/duckcrypto.h"

const int MEMORY_LOW_THRESHOLD = PACKET_LENGTH + sizeof(CdpPacket);
const int NUM_SECTORS = 312; //total desired bits divided by bits per sector
const int NUM_HASH_FUNCS = 2;
const int BITS_PER_SECTOR = 32; //size of unsigned int is 32 bits
const int MAX_MESSAGES = 100;

Duck::Duck(CDPInterface* intf, std::string name):
      filter(NUM_SECTORS, NUM_HASH_FUNCS, BITS_PER_SECTOR, MAX_MESSAGES),
      duckRadio(intf)
{
   duckName = name;
   this->intf = intf;
}

Duck::~Duck() {
   delete txPacket;
   delete rxPacket;
}

void Duck::setEncrypt(bool state) {
   duckcrypto::setEncrypt(state);
}

bool Duck::getEncrypt() {
   return duckcrypto::getState();
}

void Duck::setAESKey(uint8_t newKEY[32]) {
   duckcrypto::setAESKey(newKEY);
}

void Duck::setAESIv(uint8_t newIV[16]) {
   duckcrypto::setAESIV(newIV);
}

void Duck::encrypt(uint8_t* text, uint8_t* encryptedData, size_t inc) {
   duckcrypto::encryptData(text, encryptedData, inc);
}

void Duck::decrypt(uint8_t* encryptedData, uint8_t* text, size_t inc) {
   duckcrypto::decryptData(encryptedData, text, inc);
}

int Duck::setDeviceId(std::vector<byte> id) {
   if (id.size() != DUID_LENGTH) {
      logerror("ERROR  device id too long rc = %d", ERR_NONE);
      return ERR_ID_TOO_LONG;
   }

   duid.clear();
   duid.assign(id.begin(), id.end());
   loginfo("setupDeviceId rc = " + std::string(ERR_NONE));
   return ERR_NONE;
}

int Duck::setDeviceId(byte* id) {
   if (id == NULL) {
      return ERR_SETUP;
   }
   int len = *(&id + 1) - id;
   if (len > DUID_LENGTH) {
      return ERR_ID_TOO_LONG;
   }
   duid.assign(id, id + len);
   loginfo("setupDeviceId rc = " + std::string(ERR_NONE));

   return ERR_NONE;
}

int Duck::setupSerial(int baudRate) {
   intf->setupSerial(baudRate);
   loginfo("setupSerial rc = " + std::string(ERR_NONE));
   loginfo("Running CDP Version: " + std::string(getCDPVersion().c_str()));
   return ERR_NONE;
}

// TODO: use LoraConfigParams directly as argument to setupRadio
int Duck::setupRadio(float band, int ss, int rst, int di0, int di1,
                     int txPower, float bw, uint8_t sf, uint8_t gain) {
   LoraConfigParams config;

   config.band = band;
   config.ss = ss;
   config.rst = rst;
   config.di0 = di0;
   config.di1 = di1;
   config.txPower = txPower;
   config.bw = bw;
   config.sf = sf;
   config.gain = gain;
   config.func = Radio::onInterrupt;

   int err = duckRadio.setupRadio(config);

   if (err == LORA_ERR_BEGIN) {
      logerror("ERROR setupRadio. Starting LoRa Failed. rc = %d", err);
      return err;
   }
   if (err == LORA_ERR_SETUP) {
      logerror("ERROR setupRadio. Failed to setup Radio. rc = %d", err);
      return err;
   }
   if (err == LORA_ERR_RECEIVE) {
      logerror("ERROR setupRadio. Failed to start receive. rc = %d" , err);
      return err;
   }

   txPacket = new Packet(duid);
   rxPacket = new Packet();
   loginfo("setupRadio rc = " + std::string(ERR_NONE));

   return ERR_NONE;
}

void Duck::setSyncWord(byte syncWord) {
   duckRadio.setSyncWord(syncWord);
}

void Duck::setChannel(int channelNum) {
   duckRadio.setChannel(channelNum);
}

int Duck::sendData(byte topic, const std::string data,
                   const std::vector<byte> targetDevice, std::vector<byte> * outgoingMuid)
{

   const byte* buffer = (byte*)data.c_str();
   int err = sendData(topic, buffer, data.length(), targetDevice, outgoingMuid);
   return err;
}

int Duck::sendData(byte topic, const byte* data, int length,
                   const std::vector<byte> targetDevice, std::vector<byte> * outgoingMuid)
{
   std::vector<byte> app_data;
   app_data.insert(app_data.end(), &data[0], &data[length]);
   int err = sendData(topic, app_data, targetDevice, outgoingMuid);
   return err;
}

int Duck::sendData(byte topic, std::vector<byte> data,
                   const std::vector<byte> targetDevice, std::vector<byte> * outgoingMuid)
{
   if (topic < reservedTopic::max_reserved) {
      logerror("ERROR send data failed, topic is reserved.");
      return PACKET_ERR_TOPIC_INVALID;
   }
   if (data.size() > MAX_DATA_LENGTH) {
      logerror("ERROR send data failed, message too large: %ld bytes", data.size());
      return PACKET_ERR_SIZE_INVALID;
   }
   int err = txPacket->prepareForSending(&filter, targetDevice, this->getType(), topic, data);

   if (err != ERR_NONE) {
      return err;
   }

   err = duckRadio.sendData(txPacket->getBuffer());

   CdpPacket packet = CdpPacket(txPacket->getBuffer());

   if (err == ERR_NONE) {
      filter.bloom_add(packet.muid.data(), MUID_LENGTH);
   }

   if (!lastMessageAck) {
      loginfo("Previous `lastMessageMuid` " + duckutils::tostd::string(lastMessageMuid) +
              " was not acked. Overwriting `lastMessageMuid` with " +
              duckutils::tostd::string(packet.muid));
   }

   lastMessageAck = false;
   lastMessageMuid.assign(packet.muid.begin(), packet.muid.end());
   assert(lastMessageMuid.size() == MUID_LENGTH);
   if (outgoingMuid != NULL) {
      outgoingMuid->assign(packet.muid.begin(), packet.muid.end());
      assert(outgoingMuid->size() == MUID_LENGTH);
   }
   txPacket->reset();

   return err;
}

muidStatus Duck::getMuidStatus(const std::vector<byte> & muid) const {
   if (duckutils::isEqual(muid, lastMessageMuid)) {
      if (lastMessageAck) {
         return muidStatus::acked;
      } else {
         return muidStatus::not_acked;
      }
   } else if (muid.size() != MUID_LENGTH) {
      return muidStatus::invalid;
   } else {
      return muidStatus::unrecognized;
   }
}

// TODO: implement this using new packet format
bool Duck::reboot(void*) {
   /*
     std::string reboot = "REBOOT";
     loginfo(reboot);
     DuckRadio::getInstance()->sendPayloadStandard(reboot, "boot");
     duckesp::restartDuck();
   */
   return true;
}

// TODO: implement this using new packet format
bool Duck::imAlive(void*) {
   /*
   std::string alive = "Health Quack";
   loginfo(alive);
   DuckRadio::getInstance()->sendPayloadStandard(alive, "health");
   */
   return true;
}

int Duck::startReceive() {
   int err = duckRadio.startReceive();
   if (err != ERR_NONE) {
      logerror("ERROR Restarting Duck...");
      intf->restart();
   }
   return err;
}

int Duck::sendPong() {
   int err = ERR_NONE;
   std::vector<byte> data(1, 0);
   err = txPacket->prepareForSending(&filter, ZERO_DUID, this->getType(), reservedTopic::pong, data);
   if (err != ERR_NONE) {
      logerror("ERROR Oops! failed to build pong packet, err = %d", err);
      return err;
   }
   err = duckRadio.sendData(txPacket->getBuffer());
   if (err != ERR_NONE) {
      logerror("ERROR Oops! Radio sendData failed, err = %d", err);
      return err;
   }
   return err;
}

int Duck::sendPing() {
   int err = ERR_NONE;
   std::vector<byte> data(1, 0);
   err = txPacket->prepareForSending(&filter, ZERO_DUID, this->getType(), reservedTopic::ping, data);
   if (err != ERR_NONE) {
      logerror("ERROR Failed to build ping packet, err = %d", err);
      return err;
   }
   err = duckRadio.sendData(txPacket->getBuffer());
   if (err != ERR_NONE) {
      logerror("ERROR Radio sendData failed, err = %d", err);
   }
   return err;
}

std::string Duck::getErrorString(int error) {
   std::string errorStr = std::to_string(error) + ": ";

   switch (error) {
      case ERR_NONE:
         return errorStr + "No error";
      case ERR_NOT_SUPPORTED:
         return errorStr + "Feature not supported";
      case ERR_SETUP:
         return errorStr + "Setup failure";
      case ERR_ID_TOO_LONG:
         return errorStr + "Id length is invalid";
      case ERR_OTA:
         return errorStr + "OTA update failure";
      case LORA_ERR_BEGIN:
         return errorStr + "Radio module initialization failed";
      case LORA_ERR_SETUP:
         return errorStr + "Radio module configuration failed";
      case LORA_ERR_RECEIVE:
         return errorStr + "Radio module failed to read data";
      case LORA_ERR_TIMEOUT:
         return errorStr + "Radio module timed out";
      case LORA_ERR_TRANSMIT:
         return errorStr + "Radio moduled failed to send data";
      case LORA_ERR_HANDLE_PACKET:
         return errorStr + "Radio moduled failed to handle RX data";
      case LORA_ERR_MSG_TOO_LARGE:
         return errorStr + "Attempted to send a message larger than 256 bytes";

      case WIFI_ERR_NOT_AVAILABLE:
         return errorStr + "Wifi network is not availble";
      case WIFI_ERR_DISCONNECTED:
         return errorStr + "Wifi is disconnected";
      case WIFI_ERR_AP_CONFIG:
         return errorStr + "Wifi configuration failed";

      case DNS_ERR_STARTING:
         return errorStr + "DNS initialization failed";

      case PACKET_ERR_SIZE_INVALID:
         return errorStr + "Duck packet size is invalid";
      case PACKET_ERR_TOPIC_INVALID:
         return errorStr + "Duck packet topic field is invalid";
      case PACKET_ERR_MAX_HOPS:
         return errorStr + "Duck packet reached maximum allowed hops";

      case INTERNET_ERR_SETUP:
         return errorStr + "Internet setup failed";
      case INTERNET_ERR_SSID:
         return errorStr + "Internet SSID is not valid";
      case INTERNET_ERR_CONNECT:
         return errorStr + "Internet connection failed";
   }

   return "Unknown error";
}