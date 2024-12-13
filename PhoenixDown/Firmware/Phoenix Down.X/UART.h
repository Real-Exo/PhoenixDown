#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#define UART_PUSH(Object)   UART_Push(&Object, sizeof(Object));
#define UART_POP(Object)    UART_Pop(&Object, sizeof(Object));

void
UART_Init(void);

void
UART_Push(void *vp, uint8_t n);

void
UART_Pop(void *vp, uint8_t n);

void
UART_Tx(void);

void
UART_Rx(void);

bool
UART_TxDone(void);

bool 
UART_RxDone(void);

uint8_t
UART_BytesReceived(void);

void
UART_TxISR(void);

void
UART_RxISR(void);

#endif //UART_H