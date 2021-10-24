#include "Computer.h"

using namespace std;

//----------------------------------------------------------------------------------------------------------------------
// BEGIN: CDPInterface overrides

/**
 * Initialize gps module (that doesn't exist)
 */
void Computer::setupGPS() {
   thread dataThread(&Computer::mockDataThread, this);
   cout << "successfully set up gps";
}

/**
 * Gives raw gps data to the gps module to parse, when requested by the application end-user.
 *
 * @return the raw gps data
 */
std::vector<uint8_t> Computer::getGPSBuffer() {
   return this->rawGPSData;
}


void Computer::setupLora() {
   cout << "successfully set up LoRa";
}

std::vector<uint8_t> Computer::getLoraBuffer() {
   return std::vector<uint8_t>();
}

void Computer::acquireSemaphore() {

}

void Computer::releaseSemaphore() {

}

void Computer::getTimer() {

}

// END: CDPInterface overrides



//----------------------------------------------------------------------------------------------------------------------
// BEGIN: supporting functions

vector<uint8_t> Computer::getMockGPSValues() {
   string mockSentence = "$GPRMC,210230,A,3855.4487,N,09446.0071,W,0.0,076.2,130495,003.8,E*69";

   vector<uint8_t> retVal = vector<uint8_t>();
   copy(mockSentence.begin(), mockSentence.end(), back_inserter(retVal));

   return retVal;
}

[[noreturn]] void Computer::mockDataThread() {
   while (true) {
      this->rawGPSData = Computer::getMockGPSValues();
      cout << "got fake gps data";

      std::this_thread::sleep_for(std::chrono::seconds(1));
   }
}

// END: supporting functions
