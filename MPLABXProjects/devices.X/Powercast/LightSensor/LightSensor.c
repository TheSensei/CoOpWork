/*********************************************************************
 * Light Sensor.c
 * Written by Daniel W Harrist
 * Copyright 2015
 ********************************************************************/
#include "HardwareProfile.h"
#include "../Common/AT86RF212.h"
#include <stdlib.h>

//************************** VARIABLES ************************************
DWORD packet_count = 10;

// Sensor reads sent through radio
WORD ADC_res = 0;
WORD vdd_mv = 0;
BYTE batt = 0;
WORD batt_mv = 0;
WORD light_mv = 0;
WORD light = 0;
WORD light_sc = 0;
WORD last_light = 0;         // Last computed light level
volatile WORD proceed = 0;
volatile BYTE transmitting = 0;

BYTE high_rate = 0;          // True if we are in high rate transmission
BYTE base_rate_counter = 0;   // Counter to control base rate transmissions
BYTE quiet_scan_counter = 0; // Number of quiet scans for low rate transition

__prog__ const volatile DWORD NodeID __attribute__((space(prog), address(0x2BF0)));
//DWORD NodeID = 0x07000011;

// Sensor ADC inputs
#define REF_CHANNEL     15
#define BATT_CHANNEL    5
#define LIGHT_CHANNEL   0

// Values for calculations
#define REF_VAL         1228800UL       // Reference voltage 1200mV * 1024
#define L_BIAS          1000            // Bias res for light sensor
#define IPCE            67              // current at 100 lx, 2.7V
#define LOW_BATT        3100

// counters
DWORD timeout;
#define BIND_TO         40000
#define ADC_TO          500

#define BASE_TX_MOD     (10)       // Transmit every 3 minutes
#define HIGH_RATE_TRAN_LIMIT (4)  // Wait 4 scans before dropping tx rate
#define LUX_THRESHOLD (125)       // Sensor limit for high rate

// #define NO_BUTTON_HARDWARE (1)    // Define if we are executing on old hardware
#undef NO_BUTTON_HARDWARE

/**
 * void transmit_packet()
 *
 * Transmit a sensor packet out the radios
 */

void
transmit_packet()
{
    packet_count++;                     // increment packet count

    TRX_ClearFrame();                   // clear TX frame
    TRX_WriteData(NodeID >> 24);        // write data to frame
    TRX_WriteData(NodeID >> 16);
    TRX_WriteData(NodeID >> 8);
    TRX_WriteData(NodeID);
    TRX_WriteData(packet_count >> 16);
    TRX_WriteData(packet_count >> 8);
    TRX_WriteData(packet_count);
    TRX_WriteData(batt);
    TRX_WriteData(0x00);
    TRX_WriteData(0x00);
    TRX_WriteData(light >> 8);
    TRX_WriteData(light);

    TRX_TxPacket();                 // send frame data
    TRX_ChangeChannel(CHAN_921);    // change channel
    TRX_TxPacket();                 // send frame data
}


/*********************************************************************
* Function:         int main(void)
**********************************************************************/
int main(void)
{
    // Initialize the system
    BoardInit();

#ifndef NO_BUTTON_HARDWARE
    // sleep in a loop until button pressed
    while (proceed == 0)
        Sleep();
#endif

    Power(POWER_UP);

    DelayMs(20);                            // startup delay

    TRX_Init();                             // start radio in off state
    TRX_PowerLevel(NOFEM_POWER);                 // max output power

    // start continuous loop
    while (1)
    {
        transmitting = 1;

        // Measure Reference voltage, convert to VDD
        ADC_read(REF_CHANNEL);
        vdd_mv = (WORD)(REF_VAL / (float)ADC_res);

        // Measure battery voltage
        ADC_read(BATT_CHANNEL);
        batt_mv = (WORD)((vdd_mv * (long)ADC_res) / 1024);
        BATT_PWR = 0;                       // disable circuit
        batt_mv = batt_mv * 2;              // reading is 1/2 batt voltage
        if (batt_mv < LOW_BATT)             // report low battery
            batt = 0;
        else                                // report batt in decivolts
        {
            if ((batt_mv % 100) < 50)       // round to nearest, not down
                batt = batt_mv / 100;
            else
                batt = (batt_mv / 100) + 1;
        }

        // Measure Light
        ADC_read(LIGHT_CHANNEL);
        light_mv = (WORD)((vdd_mv * (long)ADC_res) / 1024);

        light = (DWORD)light_mv / 1.45;     // scaling factor

        if (light > 2500)                   // make sure upper 2 bits are cleared
            light = 2500;
        else if (light < 3)
            light = 0;

        // See if we've seen a big change. If so, put us into
        // high rate.  If not, count the number of consecutive
        // small change readings in case we need to transition
        // out of high rate.
        int delta = light - last_light;
        if (abs(delta) > LUX_THRESHOLD) {
            high_rate = 1;
            quiet_scan_counter = 0;
        } else {
            ++quiet_scan_counter;
        }

        if ( high_rate && quiet_scan_counter >= HIGH_RATE_TRAN_LIMIT ) {
            base_rate_counter = 0;
            high_rate = 0;
            quiet_scan_counter = 0;
        }

        if ( !high_rate ) {
            if ( base_rate_counter == 0) {
                last_light = light;
                transmit_packet();
                base_rate_counter = BASE_TX_MOD;
            } else {
                --base_rate_counter;
            }
        } else {
            last_light = light;
            transmit_packet();
        }

        Power(POWER_DOWN);

        transmitting = 0;

        Sleep();

        Power(POWER_UP);

        DelayMs(3);                         // radio startup delay

        TRX_Init();
        TRX_PowerLevel(NOFEM_POWER);             // max power

    }   // END WHILE
}   // END MAIN



/*********************************************************************
* Function:         void ADC_read(int)
**********************************************************************/
void ADC_read(int channel)
{
    int i = 0;
    timeout = 0;

    // ADC read
    AD1CHS = channel;           // set channel to measure
    AD1CON1bits.ADON = 1;       // turn ADC on
    for (i = 0; i < 50; i++);   // startup time
    AD1CON1bits.SAMP = 1;       // start sampling
    while (!AD1CON1bits.DONE){timeout++; if (timeout == ADC_TO) break; }
    ADC_res = ADC1BUF0;         // read ADC result
    AD1CON1bits.ADON = 0;       // turn ADC off
}

/*********************************************************************
* Function:         void CNInterrupt(void)
**********************************************************************/
void _ISRFAST __attribute__((interrupt, auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;          // clear the interrupt flag
    //IEC1bits.CNIE = 0;          // disable interrupt

    if (START_BTN == 1) {
        proceed = 1;
        if (transmitting == 0) { //Only delay/transmit when sleeping
            DelayMs(50); // Crude debounce
        }
    }

//  IEC1bits.CNIE = 1;            // don't enable interrupt
}
