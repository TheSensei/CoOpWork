/*********************************************************************
 * HardwareProfile.h
 * Written by Daniel W Harrist
 * Copyright 2015
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
#define RADIO_PWR           _LATB15
#define RADIO_PWR_TRIS      _TRISB15
#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01

// Transceiver Configuration
#define TRX_INT_PIN         _RA6
#define TRX_INT_TRIS        _TRISA6
#define TRX_CSn             _LATA4
#define TRX_CSn_TRIS        _TRISA4
#define TRX_RSTn            _LATB8
#define TRX_RSTn_TRIS       _TRISB8
#define TRX_SLP             _LATB7
#define TRX_SLP_TRIS        _TRISB7
#define TRX_DIG2            _RB12
#define TRX_DIG2_TRIS       _TRISB12
#define TRX_IF              _INT2IF
#define TRX_IE              _INT2IE
#define TRX2_IE             _INT1IE
#define TRX2_CSn            _LATA4

// SPI bus
#define SPI_CLK             _LATB11
#define SPI_CLK_TRIS        _TRISB11
#define SPI_MOSI            _LATB13
#define SPI_MOSI_TRIS       _TRISB13
#define SPI_MISO            _RB10
#define SPI_MISO_TRIS       _TRISB10

// Sensor Power Control
#define SENS_PWR            _LATB4
#define SENS_PWR_TRIS       _TRISB4
    
// Battery Read Control
#define BATT_PWR            _LATA2
#define BATT_PWR_TRIS       _TRISA2

// Vacancy sensor pins
#define SER_IN              _RB9
#define SER_IN_TRIS         _TRISB9
#define DIRECT_LINK         _RA1
#define DIRECT_LINK_TRIS    _TRISA1
#define MOT_CN_IE           _CN3IE
#define MOT_CN_PUEN         _CN3PUE
#define MOT_CN_PDEN         _CN3PDE

// Start/reset button control
#define START_BTN           _RA7
#define START_BTN_TRIS      _TRISA7
#define START_BTN_IE        _CN9IE
#define START_BTN_PDE       _CN9PDE

// LED pin
#define LED                 _LATA3
#define LED_TRIS            _TRISA3

// Radio Functionality Control
#define TX_ONLY
