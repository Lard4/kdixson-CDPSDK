#include "Computer.h"
#include <thread>
#include <iostream>
#include <random>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------
// BEGIN: CDPInterface overrides

/**
 * Initialize gps module (that doesn't exist)
 */
void Computer::setupGPS() {
   thread dataThread(&Computer::mockDataThread, this);
   cout << "successfully set up GPS." << endl;
   dataThread.detach();
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
   cout << "successfully set up LoRa" << endl;
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

static std::default_random_engine randomEngine;

vector<uint8_t> Computer::getMockGPSValues() {
   vector<string> mockSentences;
   mockSentences.emplace_back("$GPRMC,210230,A,3855.4487,N,09446.0071,W,0.0,076.2,130495,003.8,E*69");
   mockSentences.emplace_back("$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62");
   mockSentences.emplace_back("$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68");

   std::uniform_int_distribution<uint8_t> distribution(0, 3);
   string mockSentence = mockSentences[distribution(randomEngine)];

   vector<uint8_t> retVal = vector<uint8_t>();
   copy(mockSentence.begin(), mockSentence.end(), back_inserter(retVal));

   return retVal;
}

[[noreturn]] void Computer::mockDataThread() {
   while (true) {
      this->rawGPSData = Computer::getMockGPSValues();

      std::this_thread::sleep_for(std::chrono::seconds(1));
   }
}

// END: supporting functions
