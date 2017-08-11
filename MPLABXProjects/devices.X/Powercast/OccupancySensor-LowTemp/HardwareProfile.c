/*********************************************************************
 * HardwareProfile.c
 * Written by Daniel W Harrist
 * Copyright 2015
 ********************************************************************/
#include "Compiler.h"
#include "HardwareProfile.h"
                            
// Configuration Section - PIC24F16KA102
#pragma config BWRP = OFF
#pragma config BSS = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config FNOSC = FRC
#pragma config IESO = OFF
#pragma config POSCMOD = NONE
#pragma config OSCIOFNC = ON
#pragma config POSCFREQ = MS
#pragma config SOSCSEL = SOSCHP
#pragma config FCKSM = CSDCMD
#pragma config WDTPS = PS512
#pragma config FWPSA = PR32
#pragma config FWDTEN = ON
#pragma config BOREN = BOR0
#pragma config PWRTEN = OFF
#pragma config I2C1SEL = PRI
#pragma config BORV = V18
#pragma config MCLRE = ON
#pragma config ICS = PGx3
#pragma config COE = OFF
#pragma config BKBUG = OFF
#pragma config DSWDTPS = DSWDTPSF 
#pragma config DSWDTOSC = LPRC
#pragma config RTCOSC = SOSC
#pragma config DSBOREN = OFF
#pragma config DSWDTEN = OFF

/*********************************************************************
 * Function:        void BoardInit( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Board is initialized for P2P usage
 *
 * Overview:        This function configures the board 
 *
 * Note:            Routine to setup the processor registers
 
 This routine needs to be called before the function 
 *                  to initialize P2P stack or any other function that
 *                  operates on the stack
 ********************************************************************/
void BoardInit(void)
{
    // Disable unused peripherals
    PMD1 = 0xFFFF;
    PMD2 = 0xFFFF;
    PMD3 = 0xFFFF;
    PMD4 = 0xFFEF;                  // EE left enabled

    LED = 0;                        // turn LED off
    LED_TRIS = 0;
	
    RADIO_PWR = 0;
    RADIO_PWR_TRIS = 0;             // radio power

    SENS_PWR = 1;
    SENS_PWR_TRIS = 0;              // sensor power
	
    BATT_PWR = 0;
    BATT_PWR_TRIS = 0;              // battery power
	
    // Set up change notification interrupt for sensor
    DIRECT_LINK_TRIS = 1;
    SER_IN_TRIS = 1;
    MOT_CN_PUEN = 0;                // disable both pullup & pulldown
    MOT_CN_PDEN = 0;
    MOT_CN_IE = 1;

    // Set up change notification interrupt for button
    START_BTN_TRIS = 1;
    START_BTN_PDE = 1;
    START_BTN_IE = 1;

    // Set up change notification interrupt
    _CNIF = 0;                      // clear CN flag
    _CNIE = 1;                      // enable interrupt

    // Set up transceiver I/O
    _INT2EP = 0;                    // pos edge polarity
    TRX_IF = 0;
    TRX_IE = 1;                     // enable INT2
    TRX_INT_TRIS = 1;               // main interrupt line
    TRX_DIG2_TRIS = 1;              // IRQ_2 interrupt, RX has begun
    TRX_CSn = 0;
    TRX_CSn_TRIS = 0;
    TRX_RSTn = 0;
    TRX_RSTn_TRIS = 0;
    TRX_SLP = 0;
    TRX_SLP_TRIS = 0;

    //Ground unused IOs to prevent excess current draw @ -40C
    _LATA0 = 0;                     // slider-5 and light
    _LATA3 = 0;                     // LED
    _LATB0 = 0;                     // slider-1
    _LATB1 = 0;                     // slider-4
    _LATB2 = 0;                     // slider-3

    _TRISA0 = 0;
    _TRISA3 = 0;
    _TRISB0 = 0;
    _TRISB1 = 0;
    _TRISB2 = 0;
}

/*********************************************************************
* Function:         void Power(int)
*
* PreCondition:     none
*
* Input:		    status
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    change power state of board
*
* Note:			    
**********************************************************************/
void Power(int status)
{
    if (status == POWER_UP)
    {
        PMD1 = 0xFFF0;              // SPI1 and ADC1 enabled
        RADIO_PWR = 1;              // enable radio
        TRX_CSn = 1;
        TRX_RSTn = 1;
        TRX_SLP = 0;
    	TRX_IF = 0;                 // clear INT2 flag

        BATT_PWR = 1;
        SENS_PWR = 0;

        SPI1CON1 = 0x013E;
        SPI1STAT = 0x8000;

        AD1CON1 = 0x00E0;           // set up ADC, autoconvert, manual Sample
        AD1CON2 = 0x0000;           // int ref, 16-word buffer, mux A
        AD1CON3 = 0x1F00;           // SAMC = 31 TAD for all reads
        AD1PCFG = 0x1802;           // AN0,2,3,4,5,10 set as analog(0)
                                    // rest set as digital(1)
    							
    }
    else if (status == POWER_DOWN)
    {
        TRX_SLP = 0;
        TRX_RSTn = 0;
        TRX_CSn = 0;
        RADIO_PWR = 0;              // disable radio

        _SPIEN = 0;                 // disable SPI
		
        BATT_PWR = 0;
        SENS_PWR = 1;
    	
    	PMD1 = 0xFFFF;              // SPI1 and ADC1 disabled
    }
}
