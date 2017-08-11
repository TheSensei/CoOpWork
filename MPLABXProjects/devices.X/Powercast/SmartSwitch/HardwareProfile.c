/*********************************************************************
 * HardwareProfile.c
 * Written by Daniel W Harrist
 * Copyright 2015
 ********************************************************************/
#include "Compiler.h"
#include "HardwareProfile.h"
                            
// Configuration Section - PIC24F16KA102
#pragma config BSS = OFF
#pragma config BWRP = OFF
#pragma config GCP = OFF
#pragma config GWRP = OFF
#pragma config FNOSC = FRC
#pragma config IESO = ON
#pragma config FCKSM = CSDCMD
#pragma config POSCFREQ = MS
#pragma config OSCIOFNC = ON
#pragma config POSCMOD = NONE
#pragma config FWDTEN = ON
#pragma config FWPSA = PR32
#pragma config WDTPS = PS256
#pragma config MCLRE = ON
#pragma config BOREN = BOR0
#pragma config PWRTEN = OFF
#pragma config BKBUG = OFF
#pragma config ICS = PGx3
#pragma config DSWDTEN = OFF
#pragma config DSBOREN = OFF

void BoardInit(void)
{
    // Disable unused peripherals
    PMD1 = 0xFFF0;
    PMD2 = 0xFFFF;
    PMD3 = 0xFFFF;
    PMD4 = 0xFFEF;                  // EE left enabled

    // Setup I/O pins
    LED = 0;                        // turn LED off
    LED_TRIS = 0;

    RADIO_PWR_TRIS = 0;             // radio power

    SENS_PWR = 1;
    SENS_PWR_TRIS = 0;              // sensor power
	
    BATT_PWR_TRIS = 0;              // battery power
	
    // Set up change notification interrupt
    START_BTN_TRIS = 1;             // set to input
    CNEN1bits.CN9IE = 1;            // enable pin CN
    IFS1bits.CNIF = 0;              // clear flag
    IEC1bits.CNIE = 1;              // enable CN int

    // Set up transceiver I/O
    _INT2EP = 0;                    // pos edge polarity
    TRX_IF = 0;
    TRX_IE = 1;                     // enable INT2
    TRX_INT_TRIS = 1;               // main interrupt line
    TRX_DIG2_TRIS = 1;              // IRQ_2 interrupt, RX has begun
    TRX_SLP = 0;
    TRX_RSTn = 0;
    TRX_CSn = 0;
    TRX_CSn_TRIS = 0;
    TRX_RSTn_TRIS = 0;
    TRX_SLP_TRIS = 0;
	
    // ground cap touch pins
    CHAN4_LAT = 0;
    CHAN3_LAT = 0;
    CHAN2_LAT = 0;
    CHAN1_LAT = 0;
    CHAN0_LAT = 0;
}

void Power(int status)
{
    if (status == POWER_UP)
    {
        PMD1 = 0xFFF0;                      // SPI1 and ADC1 enabled
        RADIO_PWR = 1;                      // enable radio
        TRX_CSn = 1;
        TRX_RSTn = 1;
        TRX_SLP = 0;
        TRX_IF = 0;                         // clear INT2 flag

        SENS_PWR = 0;                       // enable sensors
        BATT_PWR = 1;

        SPI1CON1 = 0x013E;
        SPI1STAT = 0x8000;

        AD1CON1 = 0x00E0;                   // set up ADC, autoconvert, manual Sample
        AD1CON2 = 0x0000;                   // int ref, 16-word buffer, mux A
        AD1CON3 = 0x1F00;                   // SAMC = 31 TAD for all reads
        AD1PCFG = 0xFBDF;                   // AN5,10 set as analog(0)
                                            // rest set as digital(1)
    }
    else if (status == POWER_DOWN)
    {
        TRX_SLP = 0;
        TRX_RSTn = 0;
        TRX_CSn = 0;
        RADIO_PWR = 0;                      // disable radio

        SPI1STATbits.SPIEN = 0;             // disable SPI

        SENS_PWR = 1;                       // disable sensors
        BATT_PWR = 0;

        PMD1 = 0xFFFF;                      // SPI1 and ADC1 disabled
    }
}
