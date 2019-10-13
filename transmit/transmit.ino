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
} DATA;

void setup()
{
  vw_setup(2000); // Bits per sec
  vw_set_tx_pin(2);

  pinMode(clickLPin, INPUT_PULLUP);
  pinMode(clickRPin, INPUT_PULLUP);
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);
  pinMode(buttonCPin, INPUT_PULLUP);
  pinMode(buttonDPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{
  uint8_t eleVal = analogRead(elevPin);
  uint8_t aileVal = analogRead(ailePin);
  uint8_t throVal = analogRead(throPin);
  uint8_t rudderVal = analogRead(rudderPin);
  uint8_t potVal = analogRead(potPin);
  byte clickLVal = digitalRead(clickLPin);
  byte clickRVal = digitalRead(clickRPin);
  byte buttonAVal = digitalRead(buttonAPin);
  byte buttonBVal = digitalRead(buttonBPin);
  byte buttonCVal = digitalRead(buttonCPin);
  byte buttonDVal = digitalRead(buttonDPin);

  eleVal = map(eleVal, 300, 700, 0, 128);
  aileVal = map(aileVal, 300, 700, 0, 128);
  throVal = map(throVal, 300, 700, 0, 128);
  rudderVal = map(rudderVal, 300, 700, 0, 128);
  potVal = map(potVal, 0, 1024, 0, 128);

  DATA.elevator = eleVal;
  DATA.aileron = aileVal;
  DATA.throttle = throVal;
  DATA.rudder = rudderVal;
  DATA.pot = potVal;
  DATA.clickL = clickLVal;
  DATA.clickR = clickRVal;
  DATA.buttonA = buttonAVal;
  DATA.buttonB = buttonBVal;
  DATA.buttonC = buttonCVal;
  DATA.buttonD = buttonDVal;
  vw_send((uint8_t*)&DATA, sizeof(DATA));
  vw_wait_tx();
}
