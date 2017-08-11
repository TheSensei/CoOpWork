/*********************************************************************
 * HardwareProfile.c
 * Written by Daniel W Harrist
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/
#include "HardwareProfile.h"

// Configuration Section - PIC24FJ64GA002
_CONFIG2(IESO_ON & WUTSEL_LEG & FNOSC_PRI & FCKSM_CSDCMD & OSCIOFNC_ON 
    & IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & ICS_PGx2 
    & FWDTEN_ON & WINDIS_ON & FWPSA_PR32 & WDTPS_PS4096)


void BoardInit(void)
{
    // Make all pins as Digital input, disable
    AD1PCFG = 0x7FFF;  

    // Setup SPI bus to talk to radio
    _RP7R = SCK1OUT;            // Set SCK1OUT to RP7
    _RP8R = SDO1;               // Set SDO1 to RP8
    _SDI1R = 4;                 // Set SDI1 to RP4
    SPI1CON1 = 0x013E;
    SPI1STAT = 0x8000;
    
    // 1-second timer
    TMR1 = 0x0000;
    PR1 = 62500;
    T1CON = 0x8020;		// turn on timer
    _T1IF = 0;
    _T1IE = 1;
    _T1IP = TIMER_INTERRUPT_PRIORITY_LEVEL; // Set interrupt priority

    // ultrasonic 40KHz output
    _RP12R = OC1OUT;        // set PWM output to RP12
    T2CON = 0x0000;         // stops the Timer and reset control reg
                            // prescaler settings at 1:1
    TMR2 = 0x0000;          // clear contents of the timer register
    PR2 = 100;              // set period to be counted
    OC1RS = 50;             // duty cycle 50%
    OC1R = 50;              // duty cycle buffer
    OC1CON = 0x0006;		// enable OC1 in PWM mode
    T2CONbits.TON = 1; 		// start Timer
	
    // Set up lines for radio
    _INT2R = 9;             // int2 to RP9
    TRX_INTERRUPT_PRIORITY_REGISTER_BITS = RADIO_INTERRUPT_PRIORITY_LEVEL;
    TRX_INT_TRIS = 1;           // main interrupt
    TRX_DIG1_TRIS = 1;
    TRX_DIG2_TRIS = 1;          // IRQ_2 interrupt, RX has begun
    TRX_CSn_TRIS = 0;
    TRX_RSTn_TRIS = 0;
    TRX_SLP_TRIS = 0;
    
    TRX_CSn = 1;
    TRX_RSTn = 0;
    TRX_SLP = 0;
    
    LED_GREEN = 1;
    LED_GREEN_TRIS = 0;
    LED_RED = 1;
    LED_RED_TRIS = 0;
    DETECT_PIR_TRIS = 1;
    DETECT_US_TRIS = 1;
}
