/*********************************************************************
 * HardwareProfile.c
 * Written by Daniel W Harrist
 * Copyright 2016
 ********************************************************************/
#include "Compiler.h"
#include "HardwareProfile.h"

// Configuration Section - PIC24F16KA102
_FBS(BSS_OFF & BWRP_OFF)
_FGS(GCP_OFF & GWRP_OFF)
_FOSCSEL(FNOSC_FRC & IESO_ON)
_FOSC(FCKSM_CSDCMD & POSCFREQ_MS & OSCIOFNC_ON & POSCMOD_NONE )
_FWDT(FWDTEN_ON & FWPSA_PR32 & WDTPS_PS512) // .5s
_FPOR(MCLRE_ON & BOREN_BOR0 & PWRTEN_OFF)
_FICD(BKBUG_OFF & ICS_PGx3)
_FDS(DSWDTEN_OFF & DSBOREN_OFF)



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

//    MOT_READ_TRIS = 1;              // vacancy sensor

    PART_STATUS_TRIS = 1;           // partition sensor
//    _CN4PUE = 1;                    // pull pin high

    // Set up change notification interrupt
    START_BTN_TRIS = 1;             // set to input
    _CN9PDE = 1;                    // pull pin high

    // Set up transceiver I/O
    INTCON2bits.INT2EP = 0;         // pos edge polarity
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

    // ground unconnected pins
    _LATA0 = 0;
//    _LATB0 = 0;
    _LATB1 = 0;
    _LATB2 = 0;
    _TRISA0 = 0;
//    _TRISB0 = 0;
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
