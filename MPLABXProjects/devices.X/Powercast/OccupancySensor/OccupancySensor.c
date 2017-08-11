/*********************************************************************
 * Occupancy Sensor.c
 * Written by Daniel W Harrist
 * Copyright 2015
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
#define TIMER_MAX       480             // 4 minutes between heartbeat packets
#define MOTION_MAX      720             // 6 minutes of vacancy before sending another TURN_ON command
#define STARTUP_MAX     90              // 45 seconds before averaging buttons
#define TEST_MODE_TO    240             // 2 minutes of test mode LED behavior
#define NUM_OCC_TX      2               // number of TX packets

//************************** VARIABLES ************************************
// Sensor reads sent through radio
WORD ADC_res = 0;
WORD vdd_mv = 0;
SHORT batt = 0;
WORD batt_mv = 0;
WORD motion = 0;
WORD send_timer = TIMER_MAX;
WORD motion_timer = MOTION_MAX;
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
//DWORD NodeID = 0x06000021;

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
    if (RCONbits.POR == 1) {
        // sleep in a loop until button pressed
        while (START_BTN == 0)
          Sleep();

        DelayMs(100);                   // startup delay

        while (START_BTN == 1)          // wait for button release
            Sleep();
    }
#endif
    RCONbits.POR = 0;

    Power(POWER_UP);
    DelayMs(3);
    TRX_Init();
    while (TRX_SetState(RX_ON) == 0);
    // Generate a random number to use for the packet counter
    packet_count = ((DWORD)TRX_GetEightRandomBits() << 16) +
                   ((DWORD)TRX_GetEightRandomBits() <<  8) +
                   (TRX_GetEightRandomBits());
    Power(POWER_DOWN);

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

            DelayMs(50);            // debounce delay
            while (START_BTN == 1)  // wait for button release
                Sleep();
			
            LED = 0;
            test_mode = 1;
            test_mode_timer = 0;
            transmit = 2;
        }
#endif
        if (MOT_READ)
        {
            motion = 1;
            test_motion = 1;
			
            if (startup_count == STARTUP_MAX && test_mode == 0)
            {
                if (motion_timer == MOTION_MAX)
                    motion_timer = 0;               // send packet immediately
                else if (motion_timer > 2)
                    motion_timer = NUM_OCC_TX;      // reset timer so no packet sent
            }
        }

        if (test_mode == 0)
        {
            if (motion_timer < NUM_OCC_TX)
                transmit = 1;
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
            TRX_PowerLevel(NOFEM_POWER);
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
                if (transmit == 1)
                    TRX_WriteData(65);          // command for occupancy
                else if (transmit == 2)
                    TRX_WriteData(motion);      // motion detected
                else
                    TRX_WriteData(0x00);

                TRX_TxPacket();                 // send frame data
                TRX_ChangeChannel(CONNECTOR_CHAN_2);    // change channel
                DelayMs(25);                    // wait for current radio traffic to subside before attempting to TX
                TRX_TxPacket();                 // send frame data
            }

            Power(POWER_DOWN);

            motion = 0;                     // reset flags
            transmit = 0;
            send_timer = 0;
        }

        if (motion_timer < MOTION_MAX)
            motion_timer++;                 // increment read counter to max

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
