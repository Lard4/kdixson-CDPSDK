#include "DuckDetect.h"

int DuckDetect::setupWithDefaults(std::vector<byte> deviceId, std::string ssid, std::string password) {
   // int err = Duck::setupWithDefaults(deviceId, ssid, password);
   // if (err != ERR_NONE) {
   //   logerr("ERROR setupWithDefaults rc = " + String(err));
   //   return err;
   // }

   int err = setDeviceId(deviceId);
   if (err != ERR_NONE) {
      logerror("ERROR setupWithDefaults rc = %d", err);
      return err;
   }

   err = setupRadio(0,0,0,0,0,0,0,0,0);
   if (err != ERR_NONE) {
      logerror("ERROR setupWithDefaults - setupRadio rc = %d", err);
      return err;
   }

   // if (!ssid.length() != 0 && !password.length() != 0) {
   //   err = setupWifi();
   //   if (err != ERR_NONE) {
   //     logerr("ERROR setupWithDefaults - setupWifi rc = " + String(err));
   //     return err;
   //   }

   // err = setupDns();
   // if (err != ERR_NONE) {
   //   logerr("ERROR setupWithDefaults - setupDns rc = " + String(err));
   //   return err;
   // }

   // err = setupWebServer(true);
   // if (err != ERR_NONE) {
   //   logerr("ERROR setupWithDefaults - setupWebServer rc = " + String(err));
   //   return err;
   // }

   // err = setupOTA();
   // if (err != ERR_NONE) {
   //   logerr("ERROR setupWithDefaults - setupOTA src = " + String(err));
   //   return err;
   // }

   loginfo("DuckDetect setup done");
   return ERR_NONE;
}

void DuckDetect::run() {

   duckRadio.serviceInterruptFlags();

   if (Radio::getReceiveFlag()) {
      handleReceivedPacket();
   }
}

void DuckDetect::handleReceivedPacket() {

   loginfo("handleReceivedPacket()...");

   std::vector<byte> data;
   int err = duckRadio.readReceivedData(&data);

   if (err != ERR_NONE) {
      logerror("ERROR Failed to get data from DuckRadio. rc = %d", err);
      return;
   }

   if (data[TOPIC_POS] == reservedTopic::pong) {
      logdebug("run() - got ping response!");
      rssiCb(duckRadio.getRSSI());
   }
}

void DuckDetect::sendPing(bool startReceive) {
   int err = ERR_NONE;
   std::vector<byte> data(1, 0);
   err = txPacket->prepareForSending(&filter, ZERO_DUID, DuckType::DETECTOR, reservedTopic::ping, data);

   if (err == ERR_NONE) {
      err = duckRadio.sendData(txPacket->getBuffer());
      if (startReceive) {
         duckRadio.startReceive();
      }
      if (err != ERR_NONE) {
         logerror("ERROR Failed to ping, err = %d", err);
      }
   } else {
      logerror("ERROR Failed to build packet, err = %d", err);
   }
}