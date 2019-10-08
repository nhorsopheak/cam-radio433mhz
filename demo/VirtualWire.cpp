
#if defined(ARDUINO)
#if (ARDUINO < 100)
#include "WProgram.h"
#endif
#elif defined(__MSP430G2452__) || defined(__MSP430G2553__) // LaunchPad specific
#include "legacymsp430.h"
#include "Energia.h"
#else // error
#error Platform not defined
#endif

#include "VirtualWire.h"
#include <util/crc16.h>


static uint8_t vw_tx_buf[(VW_MAX_MESSAGE_LEN * 2) + VW_HEADER_LEN]
  = {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x38, 0x2c};
static uint8_t vw_tx_len = 0;
static uint8_t vw_tx_index = 0;
static uint8_t vw_tx_bit = 0;
static uint8_t vw_tx_sample = 0;
static volatile uint8_t vw_tx_enabled = 0;
static uint16_t vw_tx_msg_count = 0;
static uint8_t vw_tx_pin = 12;
//
//static uint8_t vw_ptt_pin = 10;
//static uint8_t vw_ptt_inverted = 0;

static uint8_t symbols[] =
{
  0xd,  0xe,  0x13, 0x15, 0x16, 0x19, 0x1a, 0x1c,
  0x23, 0x25, 0x26, 0x29, 0x2a, 0x2c, 0x32, 0x34
};

extern "C"
{
  void vw_set_tx_pin(uint8_t pin)
  {
    vw_tx_pin = pin;
  }

  void vw_setup(uint16_t speed)
  {

    uint16_t nticks; // number of prescaled ticks needed
    uint8_t prescaler; // Bit values for CS0[2:0]

#ifdef __AVR_ATtiny85__
    // figure out prescaler value and counter match value
    prescaler = _timer_calc(speed, (uint8_t) - 1, &nticks);
    if (!prescaler)
    {
      return; // fault
    }

    TCCR0A = 0;
    TCCR0A = _BV(WGM01); // Turn on CTC mode / Output Compare pins disconnected

    // convert prescaler index to TCCRnB prescaler bits CS00, CS01, CS02
    TCCR0B = 0;
    TCCR0B = prescaler; // set CS00, CS01, CS02 (other bits not needed)

    // Number of ticks to count before firing interrupt
    OCR0A = uint8_t(nticks);

    // Set mask to fire interrupt when OCF0A bit is set in TIFR0
    TIMSK |= _BV(OCIE0A);

#elif defined(__arm__) && defined(CORE_TEENSY)
    // on Teensy 3.0 (32 bit ARM), use an interval timer
    IntervalTimer *t = new IntervalTimer();
    t->begin(TIMER1_COMPA_vect, 125000.0 / (float)(speed));

#else // ARDUINO
    // This is the path for most Arduinos
    // figure out prescaler value and counter match value
    prescaler = _timer_calc(speed, (uint16_t) - 1, &nticks);
    if (!prescaler)
    {
      return; // fault
    }

    TCCR1A = 0; // Output Compare pins disconnected
    TCCR1B = _BV(WGM12); // Turn on CTC mode

    // convert prescaler index to TCCRnB prescaler bits CS10, CS11, CS12
    TCCR1B |= prescaler;

    // Caution: special procedures for setting 16 bit regs
    // is handled by the compiler
    OCR1A = nticks;
    // Enable interrupt
#ifdef TIMSK1
    // atmega168
    TIMSK1 |= _BV(OCIE1A);
#else
    // others
    TIMSK |= _BV(OCIE1A);
#endif // TIMSK1

#endif // __AVR_ATtiny85__

    // Set up digital IO pins
    pinMode(vw_tx_pin, OUTPUT);
    pinMode(vw_rx_pin, INPUT);
    pinMode(vw_ptt_pin, OUTPUT);
    digitalWrite(vw_ptt_pin, vw_ptt_inverted);
  }

  void vw_wait_tx()
  {
    while (vw_tx_enabled);
  }

  uint8_t vw_send(uint8_t* buf, uint8_t len)
  {
    uint8_t i;
    uint8_t index = 0;
    uint16_t crc = 0xffff;
    uint8_t *p = vw_tx_buf + VW_HEADER_LEN; // start of the message area
    uint8_t count = len + 3; // Added byte count and FCS to get total number of bytes

    if (len > VW_MAX_PAYLOAD)
      return false;

    vw_wait_tx();
    crc = _crc_ccitt_update(crc, count);
    p[index++] = symbols[count >> 4];
    p[index++] = symbols[count & 0xf];

    for (i = 0; i < len; i++)
    {
      crc = _crc_ccitt_update(crc, buf[i]);
      p[index++] = symbols[buf[i] >> 4];
      p[index++] = symbols[buf[i] & 0xf];
    }
    crc = ~crc;
    p[index++] = symbols[(crc >> 4)  & 0xf];
    p[index++] = symbols[crc & 0xf];
    p[index++] = symbols[(crc >> 12) & 0xf];
    p[index++] = symbols[(crc >> 8)  & 0xf];

    vw_tx_len = index + VW_HEADER_LEN;
    vw_tx_start();
    return true;
  }

  void vw_tx_start()
  {
    vw_tx_index = 0;
    vw_tx_bit = 0;
    vw_tx_sample = 0;
    vw_tx_enabled = true;
  }
  void vw_tx_stop()
  {
    digitalWrite(vw_tx_pin, false);
    vw_tx_enabled = false;
  }
}
