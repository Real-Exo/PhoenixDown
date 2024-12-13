#include <xc.h>
#include "Timer.h"

volatile struct Timers_Printer timers_TopPrinter = 
{ 
    .powerDown_100ms = 0, 
    .waitBoot_100ms = 0, 
    .reset_100ms = 0,
    .waitReady_100ms = 0
};

volatile struct Timers_Printer timers_BottomPrinter = 
{
    . powerDown_100ms = 0, 
    .waitBoot_100ms = 0, 
    .reset_100ms = 0,
    .waitReady_100ms = 0
};

volatile uint8_t timer_Process10ms = 0;

void
Timer_Init(void)
{
    PR2 = 124;
    T2CON = 0x27;   //1:16 pre-scaler, 1:5 post scaler, T2ON.
    
    TMR2IE = 1;
    PEIE = 1;
    GIE = 1;
}

bool
Timer_Count10msISR(void)
{
    static uint8_t count10ms = 0;
    
    if (timer_Process10ms)  { --timer_Process10ms; }
    
    if (++count10ms >= 10)
    {
        count10ms = 0;
        return true;
    }
    
    return false;
}

void
Timer_UpdatePrinterISR(volatile struct Timers_Printer* timers)
{
    if (timers->powerDown_100ms)    { --timers->powerDown_100ms; }
    if (timers->waitBoot_100ms)     { --timers->waitBoot_100ms; }
    if (timers->reset_100ms)        { --timers->reset_100ms; }
    if (timers->waitReady_100ms)    { --timers->waitReady_100ms; }
}
