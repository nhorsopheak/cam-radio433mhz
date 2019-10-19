// receiver

#include "VirtualWire.h"
#include<ServoTimer2.h>

ServoTimer2 servo1;
ServoTimer2 servo2;
ServoTimer2 servo3;
ServoTimer2 servo4;
ServoTimer2 servo5;

#define elevPin 7
#define ailePin 6
#define throPin 5
#define rudderPin 4
#define potPin 3

#define clickLPin 13
#define clickRPin 12
#define buttonAPin 11
#define buttonBPin 10
#define buttonCPin 9
#define buttonDPin 8

struct DATA {
  byte elevator;
  byte aileron;
  byte throttle;
  byte rudder;
  byte pot;
};

struct DATA *DataIn;
int n =0;
void setup()
{
  Serial.begin(9600);
  vw_setup(5000); // Bits per sec
  vw_set_rx_pin(2);
  vw_rx_start();
  servo1.attach(potPin);
  servo2.attach(rudderPin);
  servo3.attach(throPin);
  servo4.attach(ailePin);
  servo5.attach(elevPin);
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    DataIn = (struct DATA*)buf;
    servo1.write(map(DataIn->pot,0,254,1000,2000));
    servo2.write(map(DataIn->rudder,0,254,1000,2000));
    servo3.write(map(DataIn->throttle,0,254,1000,2000));
    servo4.write(map(DataIn->aileron,0,254,1000,2000));
    servo5.write(map(DataIn->elevator,0,254,1000,2000));

    Serial.print(DataIn->aileron);
    Serial.print(" - ");
    Serial.println(map(DataIn->aileron,0,254,1000,2000));
  }
}
