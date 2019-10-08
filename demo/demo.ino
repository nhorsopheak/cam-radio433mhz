//Transmiter

#include "VirtualWire.h"

struct {
  byte elevator;
  byte aileron;
  byte throttle;
  byte rudder;
  byte pot;
  byte buttonA;
  byte buttonB;
  byte buttonC;
  byte buttonD;
} DATA;

void setup()
{
  vw_setup(2000); // Bits per sec
  vw_set_tx_pin(12);
}

void loop()
{
  DATA.elevator = 10;
  DATA.aileron = 20;
  DATA.throttle = 30;
  DATA.rudder = 40;
  DATA.pot = 50;
  DATA.buttonA = 1;
  DATA.buttonB = 1;
  DATA.buttonC = 0;
  DATA.buttonD = 0;
  vw_send((uint8_t*)&DATA, sizeof(DATA));
  vw_wait_tx();
}
