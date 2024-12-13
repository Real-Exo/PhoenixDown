#ifndef SHADOW_H
#define SHADOW_H

#include <xc.h>
#include <stdint.h>

#define SHADOW_COPY() { PORTA = shadow_A; PORTB = shadow_B; }

extern uint8_t shadow_A;
extern uint8_t shadow_B;

#endif //SHADOW_H