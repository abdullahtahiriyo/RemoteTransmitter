
#include "RemoteTransmitter.h"

#define TXPIN 2

RemoteTransmitter tx=RemoteTransmitter(TXPIN, 0x6B, 0x1);


void setup() {

}


void loop() {
  
  delay(1000);
  tx.send(-8.2);
  //tx.send(9.4,true,false);
} 

