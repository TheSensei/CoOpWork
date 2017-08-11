/*
 * File:   HardwareProfile.c
 * Author: knolish
 *
 * Created on April 4, 2017, 9:32 AM
 */


#include "xc.h"
#include "Compiler.h"
#include "HardwareProfile.h"

#if defined(__PIC24F16KA102__)
// Configuration Section - PIC24F16KA102
_FBS(BSS_OFF & BWRP_OFF)
_FGS(GCP_OFF & GWRP_OFF)
_FOSCSEL(FNOSC_FRC & IESO_ON)
_FOSC(FCKSM_CSDCMD & POSCFREQ_MS & OSCIOFNC_ON & POSCMOD_NONE )
_FWDT(FWDTEN_ON & FWPSA_PR32 & WDTPS_PS512) // .5s
_FPOR(MCLRE_ON & BOREN_BOR0 & PWRTEN_OFF)
_FICD(BKBUG_OFF & ICS_PGx3)
_FDS(DSWDTEN_OFF & DSBOREN_OFF)
#elif defined(__PIC24FJ64GA002__)

// Configuration Section - PIC24FJ64GA002

// Configuration Register 2
// Two Speed Startup is Enabled
// Legacy Wake Up Timer
// Fast RC Oscillator
// Clock Switching and Fail-Safe Clock Monitor Disabled
// OSC2/CLK0/RC15 functions as port I/O
// IO Clock may be changed via unlocking sequence
// Use Default I2C1 Pin Locations (SCLI/SDA1 pins)
// Primary Oscillator Disabled

_CONFIG2(IESO_ON & WUTSEL_LEG & FNOSC_FRC & FCKSM_CSDCMD & OSCIOFNC_ON
    & IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_NONE)

// Configuration Register 1
// JTAG Port Is Disabled
// CODE Protection is Enabled
// Flash Write Protection is Disabled
// Device Resets into Operational mode, as opposed to debug mode
// COE_OFF is 0x7fff, essentially a No-Op
// Debug Emulator Pins Shared with PGC2/PGD2
// Watchdog Timer is Enabled
// Watchdog timer windowed
// Watchdog Prescaler 1:32
// Watchdog Timer Postscaler 1:8192

_CONFIG1(JTAGEN_OFF & GCP_ON & GWRP_OFF & BKBUG_OFF & COE_OFF & ICS_PGx2
    & FWDTEN_ON & WINDIS_ON & FWPSA_PR32 & WDTPS_PS8192)

// Watchdog Timer Note:
// FWSPA_PR32 and WDTPS_PS8192 above set the Watchdog timer to 8192 ms
// If change FWSPA_PR32 to FWPSA_PR128, timer will be 4 times longer (32768 ms)
// One can also change WDTPS_PS8192 to be smaller or larger
//       (e.g., max is WDTPS_PS32768 for about 32768 ms, or x4 value 131072 ms)

#else
#error "Unrecognized Processor"
#endif

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
 *
 *                  This routine needs to be called before the function
 *                  to initialize P2P stack or any other function that
 *                  operates on the stack
 ********************************************************************/
#if defined(__PIC24F16KA102__)
void
BatteryBoardInit()
{
    PMD1 = 0xFFFF;
    PMD2 = 0xFFFF;
    PMD3 = 0xFFFF;
    PMD4 = 0xFFEF;

    LED = 0;
    LED_TRIS = 0;

    RADIO_PWR = 0;
    RADIO_PWR_TRIS = 0;

    SENS_PWR = 0;
    SENS_PWR_TRIS = 0;

    BATT_PWR = 0;
    BATT_PWR_TRIS = 0;

    MOT_READ_TRIS = 0;

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
    _LATB0 = 0;
    _LATB1 = 0;
    _LATB2 = 0;
    _TRISA0 = 0;
    _TRISB0 = 0;
    _TRISB1 = 0;
    _TRISB2 = 0;
}
#endif

#ifdef __PIC24FJ64GA002__
void
ConnectorBoardInit()
{
   // Make all pins as Digital input, disable
    AD1PCFG = 0x7FFF;

    // Setup SPI bus to talk to radio
    _RP7R = SCK1OUT;            // Set SCK1OUT to RP7
    _RP8R = SDO1;               // Set SDO1 to RP8
    _SDI1R = 4;                 // Set SDI1 to RP4
    SPI1CON1 = 0x013E;
    SPI1STAT = 0x8000;

 #ifdef PROBED_CONNECTOR
    // Set up probe lines for connectors that have scope probe wires
    // attached to pins 21,22, and 23

    P21 = 0;
    P21_TRIS = 0;

    P22 = 0;
    P22_TRIS = 0;

    P23 = 0;
    P23_TRIS = 0;
#endif

    // Set up lines for radio
    RPINR1bits.INT2R = 9;       // int2 to RP9
    TRX_INTERRUPT_PRIORITY_REGISTER_BITS = RADIO_INTERRUPT_PRIORITY_LEVEL;
    TRX_IF = 0;
    TRX_IE = 1;                 // enable INT2
    TRX_INT_TRIS = 1;           // main interrupt
    TRX_DIG1_TRIS = 1;
    TRX_DIG2_TRIS = 1;          // IRQ_2 interrupt, RX has begun
    TRX_CSn_TRIS = 0;
    TRX_RSTn_TRIS = 0;
    TRX_SLP_TRIS = 0;
    TRX_TXEN_TRIS = 0;
    TRX_RXEN_TRIS = 0;

    TRX_CSn = 1;
    TRX_RSTn = 0;
    TRX_SLP = 0;
    TRX_TXEN = 0;
    TRX_RXEN = 1;

    _TRISB13 = 0;                // debug LED
    _LATB13 = 1;
}
#endif

void
BoardInit()
{
#if defined(__PIC24F16KA102__)
    BatteryBoardInit();
#elif defined(__PIC24FJ64GA002__)
    ConnectorBoardInit();
#else
#error "Board Init Not Defined for this Processor"
#endif
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


#ifdef POWER_CONTROL

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

#endif
