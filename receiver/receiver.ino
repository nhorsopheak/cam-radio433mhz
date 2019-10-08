// receiver

#include <VirtualWire.h>

struct DATA {
  byte elevator;
  byte aileron;
  byte throttle;
  byte rudder;
  byte pot;
  byte buttonA;
  byte buttonB;
  byte buttonC;
  byte buttonD;
};

struct DATA *DataIn;

void setup()
{
  Serial.begin(9600);
  vw_setup(2000); // Bits per sec
  vw_set_rx_pin(11);
  vw_rx_start();
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    DataIn = (struct DATA*)buf;
    Serial.print("Elevator:");
    Serial.println(DataIn->elevator);
    
    Serial.print("aileron:");
    Serial.println(DataIn->aileron);
    
    Serial.print("throttle:");
    Serial.println(DataIn->throttle);
    
    Serial.print("rudder:");
    Serial.println(DataIn->rudder);
    
    Serial.print("pot:");
    Serial.println(DataIn->pot);
    
    Serial.print("buttonA:");
    Serial.println(DataIn->buttonA);
    
    Serial.print("buttonB:");
    Serial.println(DataIn->buttonB);
    
    Serial.print("buttonC:");
    Serial.println(DataIn->buttonC);
    
    Serial.print("buttonD:");
    Serial.println(DataIn->buttonD);
  }
}
