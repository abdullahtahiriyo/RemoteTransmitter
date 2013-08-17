
#include "RemoteTransmitter.h"


/************
* RemoteTransmitter
************/

RemoteTransmitter::RemoteTransmitter(uint8_t pin,uint8_t id, uint8_t channel) 
{
	_pin=pin;
	_id=id;
        _chan=(channel & 0x03); // it is just two bits (never trust the user ! ;-P)
 	pinMode(_pin, OUTPUT);
}

void RemoteTransmitter::send(float temperature,bool battery, bool beep)
{
  int16_t temp= (int16_t)(temperature * 10);
  uint32_t packet= _id ;
  packet<<= 12; 
  packet|= (temp & 0x8000)==0x8000?((temp & 0x07FF) | 0x0800):(temp & 0x07FF);
  packet<<= 2;
  packet|=_chan;
  packet<<=2;
  packet|= (battery==false)?0x2:00;
  packet|= (beep==true)?0x1:00;
  
  // Checksum (summatory of nibbles +1)
  uint32_t temppacket=packet;
  uint8_t checksum= (uint8_t)temppacket;
  uint8_t i;
  for(i=1;i<7;i++)
    checksum+=(temppacket >>= 4);
  checksum-=1;
   
  temppacket=checksum; // variable reuse
  temppacket<<= 24;
  packet|=temppacket;
  
  sendFrame(_pin,packet,BITS,REPETITIONS);
}

void RemoteTransmitter::sendPacket(uint8_t pin, uint32_t packet, uint8_t nbits)
{
  //send preamble
  digitalWrite(pin, HIGH);
  delayMicroseconds(PULSEDURATION);
  digitalWrite(pin, LOW);
  delayMicroseconds(PREAMBLE);
  
  int8_t i;
  for(i=(nbits-1);i>=0;i--)
  {
    if( (packet >> i) & 0x00000001)
    {// send 1
      digitalWrite(pin, HIGH);
      delayMicroseconds(PULSEDURATION);
      digitalWrite(pin, LOW);
      delayMicroseconds(ONE);
    }  
    else
    {// send 0
      digitalWrite(pin, HIGH);
      delayMicroseconds(PULSEDURATION);
      digitalWrite(pin, LOW);
      delayMicroseconds(ZERO);    
    }
  }
}

void RemoteTransmitter::sendFrame(uint8_t pin, uint32_t packet, uint8_t nbits, uint8_t repetitions)
{
  uint8_t j=0;
  for(j=0;j<repetitions;j++)
    sendPacket(pin, packet, nbits);
}
