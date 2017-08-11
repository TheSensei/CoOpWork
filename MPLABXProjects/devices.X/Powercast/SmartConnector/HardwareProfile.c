/*********************************************************************
 * HardwareProfile.c
 * Written by Daniel W Harrist
 * Updated by Mark Brasili
 *   - Added run-time support for other connector types (e.g., Sparrow & Emerge)
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/
#include "device_type_functions.h"
#include "HardwareProfile.h"

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


void energize_bipin_on_coil(BYTE devType)
{
    if (isRevBiPinRelayConn(devType)) {
        REV_BISTABLE_RELAY_OFF = 0;
        REV_BISTABLE_RELAY_ON = 1;
    } else {
        BISTABLE_RELAY_OFF = 0;
        BISTABLE_RELAY_ON = 1;
    }
}

void energize_bipin_off_coil(BYTE devType)
{
    if (isRevBiPinRelayConn(devType)) {
        REV_BISTABLE_RELAY_ON = 0;
        REV_BISTABLE_RELAY_OFF = 1;
    } else {
        BISTABLE_RELAY_ON = 0;
        BISTABLE_RELAY_OFF = 1;
    }
}

void deenergize_bipin_relay(BYTE devType)
{
    if (isRevBiPinRelayConn(devType)) {
        REV_BISTABLE_RELAY_ON = 0;
        REV_BISTABLE_RELAY_OFF = 0;
    } else {
        BISTABLE_RELAY_ON = 0;
        BISTABLE_RELAY_OFF = 0;
    }
}

void BoardInit(BYTE devType)
{
    // Make all pins as Digital input, disable
    AD1PCFG = 0x7FFF;

    // Setup SPI bus to talk to radio
    _RP7R = SCK1OUT;            // Set SCK1OUT to RP7
    _RP8R = SDO1;               // Set SDO1 to RP8
    _SDI1R = 4;                 // Set SDI1 to RP4
    SPI1CON1 = 0x013E;
    SPI1STAT = 0x8000;

    // Relay Setup
    if (isBiPinRelayConn(devType)) {
        BISTABLE_RELAY_ON_TRIS = 0;
        BISTABLE_RELAY_OFF_TRIS = 0;
        energize_bipin_on_coil(devType);
        DelayMs(100);
        deenergize_bipin_relay(devType);
    } else {
        MOMENTARY_RELAY_ON_TRIS = 0;
        MOMENTARY_RELAY_ON = 1;               // relay to 100% ON state
    }

    // 1-second timer
    TMR1 = 0x0000;
    PR1 = 62500;
    T1CON = 0x8020;		// turn on timer
    _T1IF = 0;
    _T1IE = 1;

    if (isSparrowOutletConn(devType) || isEmergeConn(devType))
        IPC0bits.T1IP = TIMER_INTERRUPT_PRIORITY_LEVEL_07;  // Set interrupt priority - T1 should have highest priority
    else
    {
        // Timer one should have the highest priority
        IPC0bits.T1IP = TIMER_INTERRUPT_PRIORITY_LEVEL_06; // Set interrupt priority
    }

    if (isBiPinRelayConn(devType))
    {
        // Setup Timer 5 to use for driving the latching relay
        T5CON = 0x00;               // Stop Timer
        TMR5 = 0x0000;              // Clear timer
        PR5  = ((50000 * 8) / 64); // Set the period of the timer to 100 milliseconds
        // Set the pre-scale to 1:64
        T5CONbits.TCKPS0 = 0;
        T5CONbits.TCKPS1 = 1;
        IPC7bits.T5IP = TIMER_INTERRUPT_PRIORITY_LEVEL_06;  // Set Interrupt Priority
        IFS1bits.T5IF = 0;  // Clear Interrupt flag
        IEC1bits.T5IE = 1;  // Enable Timer 5 interrupts
        T5CONbits.TON = 1;
    }

    if (isDimmingConn(devType))
    {
        // Fade
        T4CON = 0x00;               // Stop Timer
        TMR4 = 0x0000;              // Clear timer
        PR4  = 0xffff;              // No interrupts
        // Set the pre-scale to 1:256
        T4CONbits.TCKPS0 = 1;
        T4CONbits.TCKPS1 = 1;
        IEC1bits.T4IE = 0;  // Disable Timer 5 interrupts
        T4CONbits.TON = 1;

        // Dimming
        _RP12R = OC1OUT;            // set PWM output to RP12
        T2CON = 0x0000;             // stops the Timer and reset control reg
                                    // prescaler settings at 1:1
        TMR2 = 0x0000;              // clear contents of the timer register

        // Note on dimming output - In most connectors, the
        // PWM output is inverted before going into the D->A conversion.
        // Thus to achieve a dim level of 40%, the PWM output has to be low
        // for 40% of the duty cycle and high for 60% of the duty cycle.

        if (isEmergeSlowerPwmConn(devType))
        {
            PR2 = PWM_VALUE;            // set period to be counted
#ifdef EMERGE_SLOWER_PWM_INVERTED
            OC1RS = 0;                  // duty cycle - 100% on
#else
            OC1RS = PWM_VALUE;          // duty cycle - 100% on
#endif
        }
        else
        {
            PR2 = 600;                  // set period to be counted
            OC1RS = 0;                  // duty cycle - 100% on
        }

        OC1R = 0;                   // duty cycle buffer
        OC1CON = 0x0006;		// enable OC1 in PWM mode
        T2CONbits.TON = 1; 		// start Timer
    }
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
