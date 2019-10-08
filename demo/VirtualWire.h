
#ifndef VirtualWire_h
#define VirtualWire_h
#include <Arduino.h>
#include <stdlib.h>

#undef abs
#undef double
#undef round

#define VW_MAX_MESSAGE_LEN 30
#define VW_MAX_PAYLOAD VW_MAX_MESSAGE_LEN-3
#define VW_RX_RAMP_LEN 160
#define VW_RX_SAMPLES_PER_BIT 8
#define VW_RAMP_INC (VW_RX_RAMP_LEN/VW_RX_SAMPLES_PER_BIT)
#define VW_RAMP_TRANSITION VW_RX_RAMP_LEN/2
#define VW_RAMP_ADJUST 9
#define VW_RAMP_INC_RETARD (VW_RAMP_INC-VW_RAMP_ADJUST)
#define VW_RAMP_INC_ADVANCE (VW_RAMP_INC+VW_RAMP_ADJUST)
#define VW_HEADER_LEN 8

extern "C"
{
    extern void vw_set_tx_pin(uint8_t pin);
    extern void vw_setup(uint16_t speed);
    extern void vw_wait_tx();
    extern uint8_t vw_send(uint8_t* buf, uint8_t len);
    extern void vw_tx_start();
    extern void vw_tx_stop();
}
#endif
