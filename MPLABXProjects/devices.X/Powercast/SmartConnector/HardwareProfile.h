/*********************************************************************
 * HardwareProfile.h
 * Written by Daniel W Harrist
 * Written by Daniel W Harrist
 * Updated by Mark Brasili
 *   - Added run-time support for other connector types (e.g., Sparrow & Emerge)
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "TimeDelay.h"
#include <math.h>

/************************ FUNCTION PROTOTYPES **********************/
void BoardInit(BYTE devType);

/************************ MACROS ***********************************/
#define CLOCK_FREQ              8000000
#define GetInstructionClock()   (CLOCK_FREQ/2)

// Transceiver Configuration
#define TRX_INT_PIN             PORTBbits.RB9
#define TRX_INT_TRIS            TRISBbits.TRISB9
#define TRX_CSn                 LATAbits.LATA4
#define TRX_CSn_TRIS            TRISAbits.TRISA4
#define TRX_RSTn                LATBbits.LATB0
#define TRX_RSTn_TRIS           TRISBbits.TRISB0
#define TRX_SLP                 LATBbits.LATB3
#define TRX_SLP_TRIS            TRISBbits.TRISB3
#define TRX_DIG1                PORTBbits.RB1
#define TRX_DIG1_TRIS           TRISBbits.TRISB1
#define TRX_DIG2                PORTBbits.RB2
#define TRX_DIG2_TRIS           TRISBbits.TRISB2
#define TRX_IF                  IFS1bits.INT2IF
#define TRX_IE                  IEC1bits.INT2IE
#define TRX2_IE                 IEC1bits.INT1IE
#define TRX2_CSn                LATBbits.LATB5
// Set interrupt priority to one since it isn't as time critical to pull
// the data out of the radio.
#define TRX_INTERRUPT_PRIORITY_REGISTER_BITS IPC7bits.INT2IP
#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01
#define TIMER_INTERRUPT_PRIORITY_LEVEL_06 0x06
#define TIMER_INTERRUPT_PRIORITY_LEVEL_07  0x07

#define TRX_FEM                 // enables FEM control
#define TRX_TXEN                LATAbits.LATA0
#define TRX_TXEN_TRIS           TRISAbits.TRISA0
#define TRX_RXEN                LATAbits.LATA1
#define TRX_RXEN_TRIS           TRISAbits.TRISA1
#define TRX_ANTSEL              LATBbits.LATB5
#define TRX_ANTSEL_TRIS         TRISBbits.TRISB5

// Relay Control
#define BISTABLE_RELAY_ON       LATAbits.LATA3
#define BISTABLE_RELAY_ON_TRIS  TRISAbits.TRISA3
#define BISTABLE_RELAY_OFF      LATAbits.LATA2
#define BISTABLE_RELAY_OFF_TRIS TRISAbits.TRISA2

#define REV_BISTABLE_RELAY_ON   LATAbits.LATA2
#define REV_BISTABLE_RELAY_OFF  LATAbits.LATA3

#define MOMENTARY_RELAY_ON      LATAbits.LATA2
#define MOMENTARY_RELAY_ON_TRIS TRISAbits.TRISA2

// Re-mappable pin functions
#define SCK1OUT                 8
#define SDO1                    7
#define U1TX                    3
#define U2TX                    5
#define OC1OUT                  18

// Emerge Slower PWM Frequency setting
// 2857 = 1.4KHz, 600 = 6.66KHz, 24000 = 150Hz
#define PWM_VALUE               2857

// Pins 21, 22, and 23 are not connected.  These are output
// pins that can be probed out and scoped, allowing software
// to have tracepoints that show up on the scope.  When
// PROBED_CONNECTOR is defined, these pins are configured as
// digital output pins.
//
//                  PIC Register Cheat Sheet
//
// PIN  Signal      Port Control Register         Pin Select Reg
// ---------------------------------------------------------------
// 21   RB13    TRISB, PORTB, LATB, ODCB Bit 13   RPOR6 Bits 8:12
// 22   RB14    TRISB, PORTB, LATB, ODCB Bit 14   RPOR7 Bits 0:4
// 23   RB15    TRISB, PORTB, LATB, ODCB Bit 15   RPOR7 Bits 8:12

// #define PROBED_CONNECTOR 1
#undef PROBED_CONNECTOR

#ifdef PROBED_CONNECTOR

#define P21 _RB13
#define P21_LAT _LATB13
#define P21_TRIS _TRISB13

#define P22 _RB14
#define P22_LAT _LATB14
#define P22_TRIS _TRISB14

#define P23 _RB15
#define P23_LAT _LATB15
#define P23_TRIS _TRISB15

#endif

// Bi-Pin, 2 coil, relay control utilities
// The relay can be in one of 3 states:
//
//  on-coil energized
//  off-coil energized
//  no coils energized
//
// The routines block the illegal configuration of having both
// coils energized simultaneously.

void energize_bipin_on_coil(BYTE devType);
void energize_bipin_off_coil(BYTE devType);
void deenergize_bipin_relay(BYTE devType);