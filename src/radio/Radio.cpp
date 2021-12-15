#include <zlib.h>
#include "Radio.h"
#include "Utils.h"


volatile uint16_t Radio::interruptFlags = 0;
volatile bool Radio::receivedFlag = false;

Radio::Radio(CDPInterface* intf) {
   this->intf = intf;
}

int Radio::setupRadio(LoraConfigParams config) {
   int rc = intf->lora->setup();

   if (rc != ERR_NONE) {
      logerror("Error while initializing LoRa driver. state = %d", rc);
      return LORA_ERR_BEGIN;
   }

   // Radio is started, we need to set all the radio parameters, before it can
   // start receiving packets
   rc = intf->lora->setFrequency(config.band);
   if (rc == LORA_ERR_INVALID_FREQUENCY) {
      logerror("frequency is invalid");
      return LORA_ERR_SETUP;
   }

   rc = intf->lora->setBandwidth(config.bw);
   if (rc == LORA_ERR_INVALID_BANDWIDTH) {
      logerror("bandwidth is invalid");
      return LORA_ERR_SETUP;
   }

   rc = intf->lora->setSpreadingFactor(config.sf);
   if (rc == LORA_ERR_INVALID_SPREADING_FACTOR) {
      logerror("spreading factor is invalid");
      return LORA_ERR_SETUP;
   }

   rc = intf->lora->setOutputPower(config.txPower);
   if (rc == LORA_ERR_INVALID_OUTPUT_POWER) {
      logerror("output power is invalid");
      return LORA_ERR_SETUP;
   }

   rc = intf->lora->setGain(config.gain);
   if (rc == LORA_ERR_INVALID_GAIN) {
      logerror("gain is invalid");
      return LORA_ERR_SETUP;
   }

   // set the interrupt handler to execute when packet tx or rx is done.
   intf->lora->setCallbackFunction(config.func);

   // set sync word to private network
   rc = intf->lora->setSyncWord(0x12);
   if (rc != ERR_NONE) {
      logerror("sync word is invalid");
      return LORA_ERR_SETUP;
   }

   rc = intf->lora->startReceive();

   if (rc != ERR_NONE) {
      logerror("Failed to start receive");
      return LORA_ERR_RECEIVE;
   }
   return ERR_NONE;
}

void Radio::setSyncWord(byte syncWord) {
   int error = intf->lora->setSyncWord(syncWord);
   if (error != ERR_NONE) {
      logerror("sync word is invalid");
   }
   intf->lora->startReceive();
}

int Radio::readReceivedData(std::vector<byte>* packetBytes) {

   int packet_length = 0;
   int err = ERR_NONE;

   packet_length = intf->lora->getPacketLength();

   if (packet_length < MIN_PACKET_LENGTH) {
      logerror("handlePacket rx data size invalid: %d", packet_length);
      return LORA_ERR_HANDLE_PACKET;
   }

   loginfo("readReceivedData() - packet length returns: %d", packet_length);

   packetBytes->resize(packet_length);
   err = intf->lora->readData(packetBytes->data(), packet_length);
   loginfo("readReceivedData() - intf->lora->readData returns: %d", err);

   Radio::setReceiveFlag(false);
   int rxState = startReceive();

   if (err != ERR_NONE) {
      logerror("readReceivedData failed. err: %d", err);
      return LORA_ERR_HANDLE_PACKET;
   }

   loginfo("Rx packet: " + duckutils::convertToHex(packetBytes->data(), packetBytes->size()));
   loginfo("Rx packet: " + duckutils::toString(*packetBytes));

   loginfo("readReceivedData: checking path offset integrity");

   // Do some sanity checks on the received packet here before we continue
   // further RadioLib v4.0.5 has a bug where corrupt packets are still returned
   // to the app despite CRC check being enabled in the radio by both sender and
   // receiver.

   byte* data = packetBytes->data();

   loginfo("readReceivedData: checking data section CRC");

   std::vector<byte> data_section;
   data_section.insert(data_section.end(), &data[DATA_POS], &data[packet_length]);
   uint32_t packet_data_crc = duckutils::toUnit32(&data[DATA_CRC_POS]);
   uint32_t computed_data_crc = crc32(0, data_section.data(), data_section.size());
   if (computed_data_crc != packet_data_crc) {
      logerror("data crc mismatch: received: %d calculated %d", packet_data_crc, computed_data_crc);
      return LORA_ERR_HANDLE_PACKET;
   }
   // we have a good packet
   loginfo("RX: rssi: %d snr: %d fe: %d size: %d",
           intf->lora->getRSSI(),
           intf->lora->getSNR(),
           intf->lora->getFrequencyError(true),
           packet_length);

   if (rxState != ERR_NONE) {
      return rxState;
   }

   return err;
}

int Radio::sendData(byte* data, int length) {
   return startTransmitData(data, length);
}

int Radio::relayPacket(Packet* packet) {
   return startTransmitData(packet->getBuffer().data(),
                            packet->getBuffer().size());
}

int Radio::sendData(std::vector<byte> data) {
   return startTransmitData(data.data(), data.size());
}

int Radio::startReceive() {
   int state = intf->lora->startReceive();

   if (state != ERR_NONE) {
      logerror("startReceive failed, code %d", state);
      return LORA_ERR_RECEIVE;
   }

   return ERR_NONE;
}

int Radio::getRSSI() { return intf->lora->getRSSI(); }

// TODO: implement this
int Radio::ping() { return ERR_NOT_SUPPORTED; }

int Radio::standBy() { return intf->lora->standby(); }

int Radio::sleep() { return intf->lora->sleep(); }

void Radio::processRadioIrq() {}

void Radio::setChannel(int channelNum) {
   logdebug("Setting Channel to: %d", channelNum);

   int err;
   switch(channelNum) {
      case 2:
         err = intf->lora->setFrequency(CHANNEL_2);
         break;

      case 3:
         err = intf->lora->setFrequency(CHANNEL_3);
         break;

      case 4:
         err = intf->lora->setFrequency(CHANNEL_4);
         break;

      case 5:
         err = intf->lora->setFrequency(CHANNEL_5);
         break;

      case 6:
         err = intf->lora->setFrequency(CHANNEL_6);
         break;

      case 1:
      default:
         err = intf->lora->setFrequency(CHANNEL_1);
         break;
   }

   if (err != ERR_NONE) {
      logerror("Failed to set channel");
   } else {
      intf->lora->startReceive();
      channel = channelNum;
      loginfo("Channel Set");
   }
}

void Radio::serviceInterruptFlags() {
   if (Radio::interruptFlags != 0) {
      /* TODO: set these constants

      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_RX_TIMEOUT) {
         loginfo("Interrupt flag was set: timeout");
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_RX_DONE) {
         loginfo("Interrupt flag was set: packet reception complete");
         Radio::setReceiveFlag(true);
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_PAYLOAD_CRC_ERROR) {
         loginfo("Interrupt flag was set: payload CRC error");
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_VALID_HEADER) {
         loginfo("Interrupt flag was set: valid header received");
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_TX_DONE) {
         loginfo("Interrupt flag was set: payload transmission complete");
         startReceive();
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_CAD_DONE) {
         loginfo("Interrupt flag was set: CAD complete");
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_FHSS_CHANGE_CHANNEL) {
         loginfo("Interrupt flag was set: FHSS change channel");
      }
      if (Radio::interruptFlags & SX127X_CLEAR_IRQ_FLAG_CAD_DETECTED) {
         loginfo("Interrupt flag was set: valid LoRa signal detected during CAD operation");
      }
       */

      Radio::interruptFlags = 0;
   }
}

// IMPORTANT: this function MUST be 'void' type and MUST NOT have any arguments!
void Radio::onInterrupt(void) {
   //TODO: fix this
   // interruptFlags = intf->lora->getIRQFlags();
}

int Radio::startTransmitData(byte* data, int length) {
   int err = ERR_NONE;
   int tx_err = ERR_NONE;
   loginfo("TX data");
   logdebug(" -> %s", duckutils::convertToHex(data, length));
   logdebug(" -> length: %d", length);

   //TODO: resolve millis() call
   long t1;// = millis();
   // this is going to wait for transmission to complete or to timeout
   // when transmit is complete, the Di0 interrupt will be triggered
   tx_err = intf->lora->transmit(data, length);
   switch (tx_err) {
      case ERR_NONE:
         loginfo("TX data done in: %d ms", (millis() - t1));
         break;

      case LORA_ERR_PACKET_TOO_LONG:
         // the supplied packet was longer than 256 bytes
         logerror("startTransmitData too long!");
         err = LORA_ERR_MSG_TOO_LARGE;
         break;

      case LORA_ERR_TX_TIMEOUT:
         logerror("startTransmitData timeout!");
         err = LORA_ERR_TIMEOUT;
         break;

      default:
         logerror("startTransmitData failed, err: %d", tx_err);
         err = LORA_ERR_TRANSMIT;
         break;
   }

   return err;
}