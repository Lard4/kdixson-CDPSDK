#include "DuckLink.h"

int DuckLink::setupWithDefaults(std::vector<byte> deviceId, std::string ssid, std::string password) {
   int err = Duck::setupWithDefaults(deviceId, ssid, password);
   if (err != ERR_NONE) {
      logerror("ERROR setupWithDefaults rc = %d", err);
      return err;
   }

   err = setupRadio(0,0,0,0,0,0,0,0,0);
   if (err != ERR_NONE) {
      logerror("ERROR setupWithDefaults rc = %d", err);
      return err;
   }

   loginfo("DuckLink setup done");
   return ERR_NONE;
}

void DuckLink::run() {

   // TODO(rolsen): duckRadio.serviceInterruptFlags();
   duckRadio.processRadioIrq();
}