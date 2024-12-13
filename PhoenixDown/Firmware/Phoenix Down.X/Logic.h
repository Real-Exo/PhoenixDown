#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>
#include <stdbool.h>

enum Logic_RebootStep 
{
    IDLE = 0,
    POWERDOWN,
    WAITBOOT,
    RESET,
    WAITREADY
};

struct Timers_Printer;

struct Logic_PrinterMemory
{
    bool rebootRequest;
    enum Logic_RebootStep rebootStep;
    uint8_t resetRetries;
    bool reprintRequest;
};

uint8_t
Logic_UpdateStatus(uint8_t oldStatus, const struct Logic_PrinterMemory* memory, bool powered, bool ready, bool disabled);

uint8_t
Logic_StatusTransmitted(uint8_t oldStatus);

void
Logic_UpdateMemory(uint8_t status, struct Logic_PrinterMemory* memory, volatile struct Timers_Printer* timers); 

#endif //LOGIC_H