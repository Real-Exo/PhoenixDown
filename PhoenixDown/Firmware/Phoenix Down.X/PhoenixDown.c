//Config
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RA5/MCLR pin function select (RA5/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOD Reset enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Code Protection bits (Program memory code protection off)

#include <xc.h>
#include <stdbool.h>
#include "Shadow.h"
#include "Timer.h"
#include "UART.h"
#include "Logic.h"

/* Commands:
 * 0x01 - Identify - Hardware should respond with "Phnx" (not 0-terminated).
 * 0x10 - Get top printer status byte.
 * 0x15 - Get bottom printer status byte.
 *        bit<0> - power
 *        bit<1> - ready
 *        bit<2> - reboots inhibited
 *        bit<3> - reprint request
 * 0x20 - Reboot top printer.
 * 0x25 - Reboot bottom printer. */

//IO
#define TP_POWERDOWN        (1U << 2)
#define TP_POWERDOWN_PORT   shadow_A
#define TP_POWERDOWN_TRIS   TRISA
#define BP_POWERDOWN        (1U << 3)
#define BP_POWERDOWN_PORT   shadow_A
#define BP_POWERDOWN_TRIS   TRISA
#define TP_RESET            (1U << 3)
#define TP_RESET_PORT       shadow_B
#define TP_RESET_TRIS       TRISB
#define BP_RESET            (1U << 0)
#define BP_RESET_PORT       shadow_B
#define BP_RESET_TRIS       TRISB
#define TP_NOTREADY         (1U << 1)
#define TP_NOTREADY_PORT    PORTA
#define TP_NOTREADY_TRIS    TRISA
#define BP_NOTREADY         (1U << 7)
#define BP_NOTREADY_PORT    PORTA
#define BP_NOTREADY_TRIS    TRISA
#define TP_NOTPOWERED       (1U << 0)
#define TP_NOTPOWERED_PORT  PORTA
#define TP_NOTPOWERED_TRIS  TRISA
#define BP_NOTPOWERED       (1U << 6)
#define BP_NOTPOWERED_PORT  PORTA
#define BP_NOTPOWERED_TRIS  TRISA
#define DISABLE             (1U << 4)
#define DISABLE_PORT        PORTB    
#define DISABLE_TRIS        TRISB

//Constants
#define TIMER_PROCESS_10MS  10

static void
Tris_Init(void)
{
    CMCON = 7;
    
    TP_POWERDOWN_TRIS &= ~TP_POWERDOWN;
    BP_POWERDOWN_TRIS &= ~BP_POWERDOWN;
    TP_RESET_TRIS &= ~TP_RESET;
    BP_RESET_TRIS &= ~BP_RESET;
    TP_NOTREADY_TRIS |= TP_NOTREADY;
    BP_NOTREADY_TRIS |= BP_NOTREADY;
    TP_NOTPOWERED_TRIS |= TP_NOTPOWERED;
    BP_NOTPOWERED_TRIS |= BP_NOTPOWERED;
    DISABLE_TRIS |= DISABLE;
}

static void
UpdateOutputs(bool disabled)
{
    if (timers_TopPrinter.powerDown_100ms && !disabled)
    {
        TP_POWERDOWN_PORT |= TP_POWERDOWN;
    }
    else
    {   
        TP_POWERDOWN_PORT &= ~TP_POWERDOWN;
    }
    
    if (timers_BottomPrinter.powerDown_100ms && !disabled)
    {
        BP_POWERDOWN_PORT |= BP_POWERDOWN;
    }
    else
    {
        BP_POWERDOWN_PORT &= ~BP_POWERDOWN;
    }
    
    if (timers_TopPrinter.reset_100ms && !disabled)
    {
        TP_RESET_PORT |= TP_RESET;
    }
    else
    {
        TP_RESET_PORT &= ~TP_RESET;
    }
    
    if (timers_BottomPrinter.reset_100ms && !disabled)
    {
        BP_RESET_PORT |= BP_RESET;
    }
    else
    {
        BP_RESET_PORT &= ~BP_RESET;
    }
    
    SHADOW_COPY();
}

void
main()
{
    Tris_Init();
    Timer_Init();
    UART_Init();
    UART_Rx();
    
    char id[] = "Phnx";
    uint8_t topPrinterStatus = 0;
    uint8_t bottomPrinterStatus = 0;
    struct Logic_PrinterMemory topPrinterMemory = 
    { 
        .rebootRequest = false, 
        .rebootStep = IDLE, 
        .resetRetries = 0,
        .reprintRequest = false 
    };
    struct Logic_PrinterMemory bottomPrinterMemory = 
    { 
        .rebootRequest = false, 
        .rebootStep = IDLE,
        .resetRetries = 0,
        .reprintRequest = false 
    };
    
    while (1)
    {
        if (!timer_Process10ms)
        {
            timer_Process10ms = TIMER_PROCESS_10MS;
            
            topPrinterStatus = Logic_UpdateStatus(topPrinterStatus, &topPrinterMemory,
                                                  !(TP_NOTPOWERED_PORT & TP_NOTPOWERED),
                                                  !(TP_NOTREADY_PORT & TP_NOTREADY),
                                                  (DISABLE_PORT & DISABLE));
            bottomPrinterStatus = Logic_UpdateStatus(bottomPrinterStatus, &bottomPrinterMemory, 
                                                     !(BP_NOTPOWERED_PORT & BP_NOTPOWERED),
                                                     !(BP_NOTREADY_PORT & BP_NOTREADY),
                                                     (DISABLE_PORT & DISABLE));

            Logic_UpdateMemory(topPrinterStatus, &topPrinterMemory, &timers_TopPrinter);
            Logic_UpdateMemory(bottomPrinterStatus, &bottomPrinterMemory, &timers_BottomPrinter); 
        }
        UpdateOutputs(DISABLE_PORT & DISABLE);
                 
        if (UART_RxDone() && UART_BytesReceived())
        {
            uint8_t command;
            UART_POP(command);
            
            switch(command)
            {
            case 0x01:  UART_Push(id, sizeof(id) - 1);
                        break;
                        
            case 0x10:  UART_PUSH(topPrinterStatus);
                        topPrinterStatus = Logic_StatusTransmitted(topPrinterStatus);
                        break;
                        
            case 0x15:  UART_PUSH(bottomPrinterStatus);
                        bottomPrinterStatus = Logic_StatusTransmitted(bottomPrinterStatus);
                        break;
                        
            case 0x20:  topPrinterMemory.rebootRequest = true;
                        break;
                        
            case 0x25:  bottomPrinterMemory.rebootRequest = true;
                        break;
                        
            default:    break;
            }
            
            UART_Tx();
            UART_Rx();
        }
    }
}
