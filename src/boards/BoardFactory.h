#ifndef CDPSDK_BOARDFACTORY_H
#define CDPSDK_BOARDFACTORY_H

#include "Computer.h"
#include "CDPInterface.h"

class BoardFactory {
public:
   static CDPInterface* createBoard();
};


#endif //CDPSDK_BOARDFACTORY_H
