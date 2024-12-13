#include "Logic.h"
#include "Timer.h"

//Masks
#define PRINTERSTATUS_MASK_POWER    (1U << 0)
#define PRINTERSTATUS_MASK_READY    (1U << 1)
#define PRINTERSTATUS_MASK_INHIBIT  (1U << 2)
#define PRINTERSTATUS_MASK_REPRINT  (1U << 3)

//Constants
#define TIMER_POWERDOWN_100MS   50
#define TIMER_WAITBOOT_100MS    250
#define TIMER_RESET_100MS       25
#define TIMER_WAITREADY_100MS   25
#define RESET_RETRIES           12

uint8_t
Logic_UpdateStatus(uint8_t oldStatus, const struct Logic_PrinterMemory* memory, bool powered, bool ready, bool disabled)
{
    uint8_t status = 0;
    
    if (oldStatus & PRINTERSTATUS_MASK_INHIBIT)
    {
        status |= PRINTERSTATUS_MASK_INHIBIT;
    }
    
    if (powered)
    {
        status |= PRINTERSTATUS_MASK_POWER;
        
        if (!(oldStatus & PRINTERSTATUS_MASK_POWER) && !memory->rebootStep)
        {
            /* If not powered on previously, but powered on now, and we are not the cause of this power cycle
             * then inhibit reboots until printer ready. (Because it will not respond to ping before it has booted). */
            status |= PRINTERSTATUS_MASK_INHIBIT;
        }
    }
    
    if (ready)
    {
        status |= PRINTERSTATUS_MASK_READY;
        status &= ~PRINTERSTATUS_MASK_INHIBIT;
    }
    
    if (disabled)
    {
        status |= PRINTERSTATUS_MASK_INHIBIT;
    }
    
    if ((oldStatus & PRINTERSTATUS_MASK_REPRINT) || memory->reprintRequest)
    {
        status |= PRINTERSTATUS_MASK_REPRINT;
    }
    
    return status;
}

uint8_t
Logic_StatusTransmitted(uint8_t oldStatus)
{
    return oldStatus &= ~PRINTERSTATUS_MASK_REPRINT;
}

void
Logic_UpdateMemory(uint8_t status, struct Logic_PrinterMemory* memory, volatile struct Timers_Printer* timers)
{
    memory->reprintRequest = false;
    
    if ((memory->rebootRequest && memory->rebootStep) || (status & PRINTERSTATUS_MASK_INHIBIT))
    {
        memory->rebootRequest = false;
    }
    
    if (memory->rebootRequest)
    {
        timers->powerDown_100ms = TIMER_POWERDOWN_100MS;
        memory->rebootRequest = false;
        memory->rebootStep = POWERDOWN;
        memory->resetRetries = 0;
        return;
    }
    
    switch(memory->rebootStep)
    {           
    case POWERDOWN: if (!timers->powerDown_100ms)
                    {
                        timers->waitBoot_100ms = TIMER_WAITBOOT_100MS;
                        memory->rebootStep = WAITBOOT;
                    }
                    break;
                    
    case WAITBOOT:  if (!timers->waitBoot_100ms)
                    {
                        timers->reset_100ms = TIMER_RESET_100MS;
                        memory->rebootStep = RESET;
                    }
                    break;
                    
    case RESET:     if (!timers->reset_100ms)
                    {
                        timers->waitReady_100ms = TIMER_WAITREADY_100MS;
                        memory->rebootStep = WAITREADY;
                    }
                    break;
                    
    case WAITREADY: if (!timers->waitReady_100ms)
                    {
                        if (status & PRINTERSTATUS_MASK_READY)
                        {
                            memory->reprintRequest = true;
                        }
                        else
                        {
                            if (++memory->resetRetries <= RESET_RETRIES)
                            {
                                timers->waitBoot_100ms = TIMER_WAITBOOT_100MS;
                                memory->rebootStep = WAITBOOT;
                                return;
                            }
                        }
                        
                        memory->rebootStep = IDLE;
                    }
                    break;
    
    default:        break;
    }
}
