#ifndef DUCK_H
#define DUCK_H

class Duck;
// Since Duck needs to know about DuckNet and DuckNet needs to know about Duck,
// this forward declaration allows a Duck pointer to be declared in DuckNet.h.
// Similarly, muidStatus needs to be declared before DuckNet.h is included.
enum muidStatus {
   invalid, // The MUID was not given in the correct format.
   unrecognized, // The MUID was not recognized. The Duck may have forgotten it.
   not_acked, // The MUID was recognized but not yet ack'd.
   acked // The MUID was recognized and has been ack'd.
};

#include "../message/BloomFilter.h"
#include "../message/Packet.h"
#include "../radio/Radio.h"
#include "DuckTypes.h"
#include "Utils.h"

class Duck {

public:
   /**
    * @brief Construct a new Duck object.
    *
    */
   Duck(CDPInterface* intf, std::string name="");

   virtual ~Duck();

   std::string getCDPVersion() { return duckutils::getCDPVersion(); }

   /**
    * @brief Set the Device Name object
    *
    * @param name
    */
   void setName(std::string name) { this->duckName = name; }

   /**
    * @brief Get the duck's name.
    *
    * @returns A string representing the duck's name
    */
   std::string getName() {return duckName;}
   /**
    * @brief setup the duck unique ID
    *
    * @param an 8 byte unique id
    * @return DUCK_ERR_NONE if successful, an error code otherwise
    */
   int setDeviceId(std::vector<byte> id);

   /**
    * @brief setup the duck unique ID
    *
    * @param an 8 byte unique id
    * @return DUCK_ERR_NONE if successful, an error code otherwise
    */
   int setDeviceId(byte* id);

   /**
    * @brief Setup serial connection.
    *
    * @param baudRate default: 115200
    */
   int setupSerial(int baudRate = 115200);

   int setupRadio(float band, int ss, int rst, int di0, int di1,
                        int txPower, float bw, uint8_t sf, uint8_t gain);

   /**
    * @brief Set sync word used to communicate between radios. 0x12 for private and 0x34 for public channels.
    *
    * @param syncWord set byte syncWord
    */
   void setSyncWord(byte syncWord);

   /**
    * @brief Set radio channel to transmit and receive on.
    *
    * @param channelNum set radio channel 1-5
    */
   void setChannel(int channelNum);

   /**
    * @brief Sends data into the mesh network.
    *
    * @param topic the message topic
    * @param data a string representing the data
    * @param targetDevice the device UID to receive the message (default is no target device)
    * @param outgoingMuid Output parameter that returns the MUID of the sent packet. NULL is ignored.
    * @return DUCK_ERR_NONE if the data was send successfully, an error code otherwise.
    */
   int sendData(byte topic, const std::string data,
                const std::vector<byte> targetDevice = ZERO_DUID, std::vector<byte> * outgoingMuid = NULL);

   /**
    * @brief Sends data into the mesh network.
    *
    * @param topic the message topic
    * @param data a vector of bytes representing the data to send
    * @param targetDevice the device UID to receive the message (default is no target device)
    * @param outgoingMuid Output parameter that returns the MUID of the sent packet. NULL is ignored.
    * @return DUCK_ERR_NONE if the data was send successfully, an error code
    otherwise.
    */
   int sendData(byte topic, std::vector<byte> bytes,
                const std::vector<byte> targetDevice = ZERO_DUID, std::vector<byte> * outgoingMuid = NULL);

   /**
    * @brief Sends data into the mesh network.
    *
    * @param topic the message topic
    * @param data a byte buffer representing the data to send
    * @param length the length of the byte buffer
    * @param targetDevice the device UID to receive the message (default is no target device)
    * @param outgoingMuid Output parameter that returns the MUID of the sent packet. NULL is ignored.
    * @return DUCK_ERR_NONE if the data was send successfully, an error code
    * otherwise.
    */
   int sendData(byte topic, const byte* data, int length,
                const std::vector<byte> targetDevice = ZERO_DUID, std::vector<byte> * outgoingMuid = NULL);

   /**
    * @brief Get the status of an MUID
    */
   muidStatus getMuidStatus(const std::vector<byte> & muid) const;

   /**
    * @brief Get an error code description.
    *
    * @param error an error code
    * @returns a string describing the error.
    */
   std::string getErrorString(int error);

   /**
    * @brief Turn on or off encryption.
    *
    * @param state true for on, false for off
    */
   void setEncrypt(bool state);

   /**
    * @brief get encryption state.
    *
    * @return true for on, false for off
    */
   bool getEncrypt();

   /**
    * @brief Turn on or off decryption. Used with MamaDuck
    *
    * @param state true for on, false for off
    */
   void setDecrypt(bool state);

   /**
    * @brief get decryption state.
    *
    * @return true for on, false for off
    */
   bool getDecrypt();

   /**
    * @brief Set new AES key for encryption.
    *
    * @param newKEY byte array, must be 32 bytes
    */
   void setAESKey(uint8_t newKEY[32]);

   /**
    * @brief Set new AES initialization vector.
    *
    * @param newIV byte array, must be 16 bytes
    */
   void setAESIv(uint8_t newIV[16]);

   /**
    * @brief Encrypt data using AES-256 CTR.
    *
    * @param text pointer to byte array of plaintext
    * @param encryptedData pointer to byte array to store encrypted message
    * @param inc size of text to be encrypted
    */
   void encrypt(uint8_t* text, uint8_t* encryptedData, size_t inc);

   /**
    * @brief Decrypt data using AES-256 CTR.
    *
    * @param encryptedData pointer to byte array to be decrypted
    * @param text pointer to byte array to store decrypted plaintext
    * @param inc size of text to be decrypted
    */
   void decrypt(uint8_t* encryptedData, uint8_t* text, size_t inc);

protected:
   Duck(Duck const&) = delete;
   Duck& operator=(Duck const&) = delete;

   std::string duckName="";

   std::string deviceId;
   std::vector<byte> duid;

   CDPInterface* intf;

   Radio duckRadio;

   Packet* txPacket = NULL;
   Packet* rxPacket = NULL;
   std::vector<byte> lastMessageMuid;

   bool lastMessageAck = true;
   // Since this may be used to throttle outgoing packets, start out in a state
   // that indicates we're not waiting for a ack

   BloomFilter filter;

   /**
    * @brief sends a pong message
    *
    * @return DUCK_ERR_NONE if successfull. An error code otherwise
    */
   int sendPong();

   /**
    * @brief sends a ping message
    *
    * @return DUCK_ERR_NONE if successfull. An error code otherwise
    */
   int sendPing();

   /**
    * @brief Tell the duck radio to start receiving packets from the mesh network
    *
    * @return DUCK_ERR_NONE if successful, an error code otherwise
    */
   int startReceive();

   /**
    * @brief Implement the duck's specific behavior.
    *
    * This method must be implemented by the Duck's concrete classes such as DuckLink, MamaDuck,...
    */
   virtual void run() = 0;

   /**
    * @brief Setup a duck with default settings
    *
    * The default implementation simply initializes the serial interface.
    * It can be overriden by each concrete Duck class implementation.
    */
   virtual int setupWithDefaults(std::vector<byte> deviceId, std::string ssid, std::string password) {
      int err = setupSerial();
      if (err != ERR_NONE) {
         return err;
      }
      err = setDeviceId(deviceId);
      if (err != ERR_NONE) {
         return err;
      }
      return ERR_NONE;
   }

   /**
    * @brief Get the duck type.
    *
    * @returns A value representing a DuckType
    */
   virtual int getType() = 0;

   static bool imAlive(void*);
   static bool reboot(void*);
};

#endif