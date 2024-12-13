#ifndef TIMER_H
#define TIMER_H

//WARNING: If there's ever a timer wider then 8-bit keep atomicity in mind! 

#include <stdint.h>
#include <stdbool.h>

struct Timers_Printer
{
    uint8_t powerDown_100ms;
    uint8_t waitBoot_100ms;
    uint8_t reset_100ms;
    uint8_t waitReady_100ms;
};

extern volatile struct Timers_Printer timers_TopPrinter;
extern volatile struct Timers_Printer timers_BottomPrinter;
extern volatile uint8_t timer_Process10ms;

void
Timer_Init(void);

bool
Timer_Count10msISR(void);

void
Timer_UpdatePrinterISR(volatile struct Timers_Printer* timers);

#endif //TIMER_H