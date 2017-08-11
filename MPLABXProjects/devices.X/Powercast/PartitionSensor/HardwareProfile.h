/*********************************************************************
 * HardwareProfile.h
 * Written by Daniel W Harrist
 * Copyright 2016
 ********************************************************************/

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "TimeDelay.h"

/************************ FUNCTION PROTOTYPES **********************/
void BoardInit(void);
void Power(int);

/************************ MACROS ***********************************/
#define POWER_UP            0
#define POWER_DOWN          1

#define CLOCK_FREQ          8000000
#define GetInstructionClock()	(CLOCK_FREQ/2)

// Radio Power Control
#define RADIO_PWR           LATBbits.LATB15
#define RADIO_PWR_TRIS      TRISBbits.TRISB15
#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01

// Transceiver Configuration
#define TRX_INT_PIN         PORTAbits.RA6
#define TRX_INT_TRIS        TRISAbits.TRISA6
#define TRX_CSn             LATAbits.LATA4
#define TRX_CSn_TRIS        TRISAbits.TRISA4
#define TRX_RSTn            LATBbits.LATB8
#define TRX_RSTn_TRIS       TRISBbits.TRISB8
#define TRX_SLP             LATBbits.LATB7
#define TRX_SLP_TRIS        TRISBbits.TRISB7
#define TRX_DIG2            PORTBbits.RB12
#define TRX_DIG2_TRIS       TRISBbits.TRISB12
#define TRX_IF              IFS1bits.INT2IF
#define TRX_IE              IEC1bits.INT2IE
#define TRX2_IE             IEC1bits.INT1IE
#define TRX2_CSn            LATAbits.LATA4

// SPI bus
#define SPI_CLK             LATBbits.LATB11
#define SPI_CLK_TRIS        TRISBbits.TRISB11
#define SPI_MOSI            LATBbits.LATB13
#define SPI_MOSI_TRIS       TRISBbits.TRISB13
#define SPI_MISO            PORTBbits.RB10
#define SPI_MISO_TRIS       TRISBbits.TRISB10

// Sensor Power Control
#define SENS_PWR            LATBbits.LATB4
#define SENS_PWR_TRIS       TRISBbits.TRISB4

// Battery Read Control
#define BATT_PWR            LATAbits.LATA2
#define BATT_PWR_TRIS       TRISAbits.TRISA2

// Vacancy sensor pin
//#define MOT_READ            PORTAbits.RA1
//#define MOT_READ_TRIS       TRISAbits.TRISA1

//Partition sensor pin
#define PART_STATUS         PORTBbits.RB0
#define PART_STATUS_TRIS    TRISBbits.TRISB0

// Start/reset button control
#define START_BTN           PORTAbits.RA7
#define START_BTN_TRIS      TRISAbits.TRISA7

// LED pin
#define LED                 LATAbits.LATA3
#define LED_TRIS            TRISAbits.TRISA3

#define TX_ONLY