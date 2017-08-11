/*********************************************************************
 * HardwareProfile.h
 * Written by Daniel W Harrist
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "TimeDelay.h" 
#include <math.h>
    
/************************ FUNCTION PROTOTYPES **********************/
void BoardInit(void);

/************************ MACROS ***********************************/
#define CLOCK_FREQ              8000000
#define GetInstructionClock()   (CLOCK_FREQ/2)

// Transceiver Configuration
#define TRX_INT_PIN             _RB9
#define TRX_INT_TRIS            _TRISB9
#define TRX_CSn                 _LATA4
#define TRX_CSn_TRIS            _TRISA4
#define TRX_RSTn                _LATB0
#define TRX_RSTn_TRIS           _TRISB0
#define TRX_SLP                 _LATB3
#define TRX_SLP_TRIS            _TRISB3
#define TRX_DIG1                _RB1
#define TRX_DIG1_TRIS           _TRISB1
#define TRX_DIG2                _RB2
#define TRX_DIG2_TRIS           _TRISB2
#define TRX_IF                  _INT2IF
#define TRX_IE                  _INT2IE
#define TRX2_IE                 _INT1IE
#define TRX2_CSn                _LATA3
// Set interrupt priority to one since it isn't as time critical to pull
// the data out of the radio.
#define TRX_INTERRUPT_PRIORITY_REGISTER_BITS IPC7bits.INT2IP
#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01
#define TIMER_INTERRUPT_PRIORITY_LEVEL 0x06

#define TRX_FEM                 // enables FEM control
#define TRX_TXEN                _LATA0
#define TRX_TXEN_TRIS           _TRISA0
#define TRX_RXEN                _LATA1
#define TRX_RXEN_TRIS           _TRISA1
#define TRX_ANTSEL              _LATA2
#define TRX_ANTSEL_TRIS         _TRISA2

// IO
#define LED_GREEN               _LATB6
#define LED_GREEN_TRIS          _TRISB6
#define LED_RED                 _LATB5
#define LED_RED_TRIS            _TRISB5
#define DETECT_PIR_PIN          (_RB14 == 1)
#define DETECT_PIR_TRIS         _TRISB14
#define DETECT_US_PIN           (_RB15 == 0)
#define DETECT_US_TRIS          _TRISB15

// Re-mappable pin functions
#define SCK1OUT                 8
#define SDO1                    7
#define OC1OUT                  18

#define TX_ONLY