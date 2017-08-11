/*********************************************************************
 * Partition Sensor.c
 * Written by Daniel W Harrist
 * & Kieran Peleaux
 * Copyright 2016
 ********************************************************************/
#include "HardwareProfile.h"
#include "../Common/AT86RF212.h"

//************************** FUNCTIONS ************************************
void Power(int);
void ADC_read(int);

//************************** CONSTANTS ************************************
// Sensor ADC inputs
#define REF_CHANNEL     10
#define BATT_CHANNEL    5

// Values for calculations
#define REF_VAL         1280000UL       // Reference voltage 1250mV * 1024
#define LOW_BATT        3100
#define TIMER_MAX       240             // 2 minutes between heartbeat packets
#define PART_MAX        240             // 2 minutes of closed (vacant) partition before sending TURN_ON command
#define STARTUP_MAX     90              // 45 seconds before averaging buttons
#define TEST_MODE_TO    240             // 2 minutes of test mode LED behavior
#define PART_POLL_MAX   1               // 1 second of debounce for partition state change

//************************** VARIABLES ************************************
// Sensor reads sent through radio
WORD ADC_res = 0;
WORD vdd_mv = 0;
SHORT batt = 0;
WORD batt_mv = 0;
WORD part_closed = 0;
WORD send_timer = TIMER_MAX;
WORD part_poll_timer = PART_POLL_MAX;
WORD state_changed = 0;
WORD send_count = 0;
WORD startup_count = 0;
WORD proceed = 0;
WORD test_mode = 0;
DWORD test_mode_timer = 0;
WORD test_motion = 0;
WORD test_prev_motion = 0;
DWORD packet_count = 0;
BYTE transmit = 0;

__prog__ const volatile DWORD NodeID __attribute__((space(prog), address(0x2bf0)));
//DWORD NodeID = 0x15000001;

// counters
DWORD timeout;
#define ADC_TO	500

#define BUTTON


/*********************************************************************
* int main(void)
**********************************************************************/
int main(void)
{
    // Initialize the system
    BoardInit();
#ifdef BUTTON
    // sleep in a loop until button pressed
    while (START_BTN == 0)
        Sleep();

    DelayMs(100);                   // startup delay

    while (START_BTN == 1)          // wait for button release
        Sleep();
#endif
    // start continuous loop
    while (1)
    {
        send_timer++;

        if (startup_count < STARTUP_MAX)
            startup_count++;
#ifdef BUTTON
        if (START_BTN == 1)         // check for button press
        {
            LED = 1;

            DelayMs(100);           // debounce delay
            while (START_BTN == 1)  // wait for button release
                Sleep();

            LED = 0;
            test_mode = 1;
            test_mode_timer = 0;
        }
#endif


        if (part_poll_timer == PART_POLL_MAX)   // poll once every second
        {
            state_changed = 0;      // only reset every 2nd pass-through to ensure transmit block is executed twice
            SENS_PWR = 0;           // enable power to partition switch
            DelayMs(3);             // startup delay

            if (PART_STATUS == part_closed)
            {
                 if (startup_count == STARTUP_MAX && test_mode == 0)
                 {
                     state_changed = 1;    // send packet immediately
                     test_motion = 1;
                 }

                     part_closed = !PART_STATUS;
            }

            SENS_PWR = 1;           // disable power to partition switch

        }

        if (test_mode == 0)
        {
            if (state_changed == 1)
                transmit = 2;
            else if (send_timer >= TIMER_MAX)
                transmit = 2;
        }
#ifdef BUTTON
        else if (test_mode == 1)
        {
            if (test_motion == 1 && test_prev_motion == 0)
                LED = 1;
            else if (test_motion == 1 && test_prev_motion == 1)
                LED ^= 1;
            else
                LED = 0;
        }

        if (test_mode == 1 && test_mode_timer < TEST_MODE_TO)
        {
            test_mode_timer++;
            test_prev_motion = test_motion;
            test_motion = 0;
        }

        if (test_mode_timer == TEST_MODE_TO) // reset test mode
        {
            LED = 0;
            test_mode = 0;
            test_mode_timer = 0;
            test_motion = 0;
            test_prev_motion = 0;
        }
#endif
        if (transmit)
        {
            Power(POWER_UP);

            DelayMs(3);                     // startup delay

            TRX_Init();                     // start radio in off state
            TRX_PowerLevel(PLUS_5);
            if (TRX_SetState(RX_ON))
            {
                DelayMs(25);                // wait for current radio traffic to subside before attempting to TX

                // Measure Reference voltage, convert to VDD
                ADC_read(REF_CHANNEL);
                vdd_mv = (WORD)(REF_VAL / (float)ADC_res);

                // Measure battery voltage
                ADC_read(BATT_CHANNEL);
                batt_mv = (WORD)((vdd_mv * (long)ADC_res) / 1024);
                BATT_PWR = 0;                   // disable circuit
                batt_mv = batt_mv * 2;          // reading is 1/2 batt voltage
                if (batt_mv < LOW_BATT)         // report low battery
                    batt = 0;
                else                            // report batt in decivolts
                {
                    if ((batt_mv % 100) < 50)   // round to nearest, not down
                        batt = batt_mv / 100;
                    else
                        batt = (batt_mv / 100) + 1;
                }

                packet_count++;

                TRX_ClearFrame();               // clear TX frame
                TRX_WriteData(NodeID >> 24);    // write data to frame
                TRX_WriteData(NodeID >> 16);
                TRX_WriteData(NodeID >> 8);
                TRX_WriteData(NodeID);
                TRX_WriteData(packet_count >> 16);
                TRX_WriteData(packet_count >> 8);
                TRX_WriteData(packet_count);
                TRX_WriteData(batt);
                TRX_WriteData(0x00);
                if (transmit == 2)
                    TRX_WriteData(part_closed); // send partition state command
                else
                    TRX_WriteData(0x00);

                TRX_TxPacket();                 // send frame data
                TRX_ChangeChannel(CHAN_921);    // change channel
                DelayMs(25);                    // wait for current radio traffic to subside before attempting to TX
                TRX_TxPacket();                 // send frame data
            }

            Power(POWER_DOWN);

            transmit = 0;
            send_timer = 0;
        }

        if (part_poll_timer < PART_POLL_MAX)
            part_poll_timer++;
        else
            part_poll_timer = 0;

        Sleep();

    }   // END WHILE

    return 0;                               // should never get here

}   // END MAIN

/*****************************************
 * Function prototypes
 *****************************************/
void ADC_read(int channel)
{
    int i = 0;
    timeout = 0;

    // ADC read
    AD1CHS = channel;               // set channel to measure
    AD1CON1bits.ADON = 1;           // turn ADC on
    for (i = 0; i < 50; i++);       // startup time
    AD1CON1bits.SAMP = 1;           // start sampling
    while (!AD1CON1bits.DONE){timeout++; if (timeout == ADC_TO) break; }
    ADC_res = ADC1BUF0;             // read ADC result
    AD1CON1bits.ADON = 0;           // turn ADC off
}
