#include "BoardFactory.h"

#define COMPUTER

CDPInterface* BoardFactory::createBoard() {
#ifdef RP2040
   return new RP2040();
#endif

#ifdef COMPUTER
   return new Computer();
#endif

   // all else fails, we know nothing about our hardware. bad.
   return nullptr;
}
