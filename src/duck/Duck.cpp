#include "../boards/BoardFactory.h"
#include "Duck.h"

Duck::Duck() {
   this->interface = BoardFactory::createBoard();
   this->gps = nullptr;
}

GPS* Duck::startGPS(uint8_t maxNmeaSentences) {
   this->gps = new GPS(this->interface, maxNmeaSentences);
   return this->gps;
}

void Duck::stopGPS() {
   delete this->gps;
   this->gps = nullptr;
}

GPS* Duck::getGPS() {
   return this->gps;
}
