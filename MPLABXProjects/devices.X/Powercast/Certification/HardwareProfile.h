/* Microchip Technology Inc. and its subsidiaries.  You may use this software
 * and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File:
 * Author:
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.


#ifndef HAS_FEM
#error "The macro HAS_FEM must be defined"
#endif

#if HAS_FEM == 1
#undef BUTTON
#undef POWER_CONTROL
#undef HAS_LED
#else
#define BUTTON
#define POWER_CONTROL
#define HAS_LED
#endif

void BoardInit();

#ifdef POWER_CONTROL
void Power(int status);

#define POWER_UP            0
#define POWER_DOWN  1
#endif

#ifdef __PIC24F16KA102__

#define CLOCK_FREQ              8000000
#define GetInstructionClock()   (CLOCK_FREQ/2)

// Radio Power Control
#define RADIO_PWR           LATBbits.LATB15
#define RADIO_PWR_TRIS      TRISBbits.TRISB15
#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01

// Transceiver Configuration
#define TRX_INT_PIN             _RA6
#define TRX_INT_TRIS            _TRISA6
#define TRX_CSn                 _LATA4
#define TRX_CSn_TRIS            _TRISA4
#define TRX_RSTn                _LATB8
#define TRX_RSTn_TRIS           _TRISB8
#define TRX_SLP                 _LATB7
#define TRX_SLP_TRIS            _TRISB7
#define TRX_DIG2                _RB12
#define TRX_DIG2_TRIS           _TRISB12
#define TRX_IF                  _INT2IF
#define TRX_IE                  _INT2IE
#define TRX2_IE                 _INT1IE
#define TRX2_CSn                _LATA4

#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01

// Sensor Power Control
#define SENS_PWR            LATBbits.LATB4
#define SENS_PWR_TRIS       TRISBbits.TRISB4

// Battery Read Control
#define BATT_PWR            LATAbits.LATA2
#define BATT_PWR_TRIS       TRISAbits.TRISA2

// Vacancy sensor pin
#define MOT_READ            PORTAbits.RA1
#define MOT_READ_TRIS       TRISAbits.TRISA1

// Start/reset button control
#define START_BTN           PORTAbits.RA7
#define START_BTN_TRIS      TRISAbits.TRISA7

// LED pin
#define LED                 LATAbits.LATA3
#define LED_TRIS            TRISAbits.TRISA3

// Radio Functionality Control
#define TX_ONLY

#elif defined(__PIC24FJ64GA002__)

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

#endif

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>

    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>

    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C
    // linkage so the functions can be used by the c code.

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
