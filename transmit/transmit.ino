//Transmiter

#include "VirtualWire.h"

#define elevPin A0
#define ailePin A1
#define throPin A2
#define rudderPin A3
#define potPin A4

#define clickLPin 12
#define clickRPin 11
#define buttonAPin 10
#define buttonBPin 9
#define buttonCPin 8
#define buttonDPin 7

struct {
  byte elevator;
  byte aileron;
  byte throttle;
  byte rudder;
  byte pot;
} DATA;

void setup()
{
  vw_setup(5000); // Bits per sec
  vw_set_tx_pin(2);

  Serial.begin(9600);
}

void loop()
{
  int eleVal = analogRead(elevPin);
  int aileVal = analogRead(ailePin);
  int throVal = analogRead(throPin);
  int rudderVal = analogRead(rudderPin);
  int potVal = analogRead(potPin);
  eleVal = map(eleVal, 0, 1023, 0, 254);
  aileVal = map(aileVal, 0, 1023, 0, 254);
  throVal = map(throVal, 0, 1023, 0, 254);
  rudderVal = map(rudderVal, 0, 1023, 0, 254);
  potVal = map(potVal, 0, 1023, 0, 254);
  
  DATA.elevator = eleVal;
  DATA.aileron = aileVal;
  DATA.throttle = throVal;
  DATA.rudder = rudderVal;
  DATA.pot = potVal;
  vw_send((uint8_t*)&DATA, sizeof(DATA));
  vw_wait_tx();
}
