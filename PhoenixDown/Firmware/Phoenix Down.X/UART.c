#include <xc.h>
#include "UART.h"

/* Original target: PIC16F628 @ 4Mhz INTOSC
 * Packet format:
 * <STP>Databytes...<Checksum><ENP>
 * where: - STP = Start of packet
 *        - ENP = End of packet
 *        - DES = Data escape for control characters
 *        - Checksum = One's complement + 1 of the least significant byte of databytes sum */

//IO
#define UART_TRIS   TRISB
#define TX          (1U << 2)
#define RX          (1U << 1)

//Constants
#define STP         0x55
#define ENP         0xAA
#define DES         0x5A
#define TX_BUFFSIZE 4
#define RX_BUFFSIZE 4

static uint8_t popIndex = 0;
static volatile uint8_t bytesToSend = 0;
static volatile uint8_t bytesReceived = 0;
static volatile uint8_t txBuff[TX_BUFFSIZE];
static volatile uint8_t rxBuff[RX_BUFFSIZE];

void
UART_Init(void)
{
    UART_TRIS |= (TX | RX);
    
    SPBRG = 25;     //9600 baud in high speed mode.
    TXSTA = 0x24;   //8-bit TX, TX enabled, Async, high speed.
    RCSTA = 0x90;   //UART enabled, 8-bit RX, continuous receive, no address detection.
    
    TXIE = 0;
    RCIE = 0;
    PEIE = 1;
    GIE = 1;
}

void
UART_Push(void* vp, uint8_t n)
{
    uint8_t* p = (uint8_t*) vp;
    
    while (n--)
    {
        txBuff[bytesToSend++] = *p++;
    }
}

void
UART_Pop(void* vp, uint8_t n)
{
    uint8_t* p = (uint8_t*) vp;
    
    while (n--)
    {
        *p++ = rxBuff[popIndex++];
    }
}

void
UART_Tx(void)
{
    TXIE = 1;
}

void
UART_Rx(void)
{
    popIndex = 0;
    RCIE = 1;
}

bool
UART_TxDone(void)
{
    return !TXIE;
}

bool
UART_RxDone(void)
{
    return !RCIE;
}

uint8_t
UART_BytesReceived(void)
{
    return bytesReceived;
}

void
UART_TxISR(void)
{
    static bool started = false;
    static bool stuffedChar;
    static uint8_t checksum;
    static uint8_t bytesSent;
    
    if (!started)
    {
        TXREG = STP;
        started = true;
        stuffedChar = false;
        checksum = 0;
        bytesSent = 0;
        return;
    }
    
    if (bytesSent == bytesToSend)
    {
        TXREG = ~checksum + 1;
        ++bytesSent;
        return;
    }
    
    if (bytesSent > bytesToSend)
    {
        TXREG = ENP;
        started = false;
        bytesToSend = 0;
        TXIE = 0;
        return;
    }
    
    uint8_t byte = txBuff[bytesSent];
    switch(byte)
    {
    case STP:
    case ENP:
    case DES:   if (!stuffedChar)
                {
                    TXREG = DES;
                    stuffedChar = true;
                    return;
                }
    
    default:    TXREG = byte;
                stuffedChar = false;
                checksum += byte;
                ++bytesSent;
                return;
    }
}

void
UART_RxISR(void)
{
    static bool started = false;
    static bool stuffedChar = false;
    static uint8_t checksum;
    
    uint8_t status = RCSTA;
    uint8_t byte = RCREG;
    
    if ((status & _RCSTA_FERR_MASK) || (status & _RCSTA_OERR_MASK))
    {
        //Framing or overrun error.
        CREN = 0;
        CREN = 1;
        started = false;
        return;
    }
    
    if (!stuffedChar)
    {
        switch(byte)
        {
        case STP:   started = true;
                    checksum = 0;
                    bytesReceived = 0;
                    return;
                    
        case DES:   stuffedChar = true;
                    return;
                    
        case ENP:   if (started && !checksum)
                    {
                        RCIE = 0;
                    }
                    return;
        }
    }
    
    stuffedChar = false;
    
    if (started)
    {
        if (bytesReceived < RX_BUFFSIZE)
        {
            rxBuff[bytesReceived++] = byte;
            checksum += byte;
        }
        else
        {
            started = false;
        }
    }
}
