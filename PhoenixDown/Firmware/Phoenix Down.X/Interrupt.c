#include <xc.h>
#include "Timer.h"
#include "UART.h"

void __interrupt()
ISR(void)
{
    if (TMR2IE && TMR2IF)
    {
        if (Timer_Count10msISR())
        {
            Timer_UpdatePrinterISR(&timers_TopPrinter);
            Timer_UpdatePrinterISR(&timers_BottomPrinter);
        }
        
        TMR2IF = 0;
    }
    
    if (TXIE && TXIF) { UART_TxISR(); TXIF = 0; }
    if (RCIE && RCIF) { UART_RxISR(); RCIF = 0; }
}
