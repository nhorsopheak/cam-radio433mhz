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
  uint8_t elevator;
  uint8_t aileron;
  uint8_t throttle;
  uint8_t rudder;
  uint8_t pot;
  byte clickR;
  byte clickL;
  byte buttonA;
  byte buttonB;
  byte buttonC;
  byte buttonD;
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
  pinMode(clickLPin,OUTPUT);
  pinMode(clickRPin,OUTPUT);
  pinMode(buttonAPin,OUTPUT);
  pinMode(buttonBPin,OUTPUT);
  pinMode(buttonCPin,OUTPUT);
  pinMode(buttonDPin,OUTPUT);
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    DataIn = (struct DATA*)buf;
    servo1.write(map(DataIn->pot,0,255,750,2250));
    servo2.write(map(DataIn->rudder,0,255,750,2250));
    servo3.write(map(DataIn->throttle,0,255,750,2250));
    servo4.write(map(DataIn->aileron,0,255,750,2250));
    servo5.write(map(DataIn->elevator,0,255,750,2250));

    digitalWrite(clickLPin,!DataIn->clickL);
    digitalWrite(clickRPin,!DataIn->clickR);
    digitalWrite(buttonAPin,!DataIn->buttonA);
    digitalWrite(buttonBPin,!DataIn->buttonB);
    digitalWrite(buttonCPin,!DataIn->buttonC);
    digitalWrite(buttonDPin,!DataIn->buttonD);


    Serial.println(n++);
  }
}
