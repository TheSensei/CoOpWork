/*********************************************************************
 * main.c
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

//************************ HEADERS ****************************************
#include "HardwareProfile.h"
#include "../Common/AT86RF212.h"

//************************** VARIABLES ************************************
__prog__ const volatile DWORD myNodeID __attribute__((space(prog),address(0xABF0)));
//DWORD myNodeID = 0x14000333;

#define HEARTBEAT_MAX_SECONDS       300
#define NO_MOTION_MAX_SECONDS       120
#define STARTUP_SECONDS             30
#define RADIO_CAL_SECONDS           300
#define OCC_PKT_DELAY_MILLI_SECS    500
#define NUM_OCC_TX                  2               // number of TX packets

WORD heartBeatTimer = HEARTBEAT_MAX_SECONDS;
WORD motionTimer = NO_MOTION_MAX_SECONDS;
WORD radioCalTimer = 0;
WORD startupTimer = 0;
WORD LEDoffCounter = 0;

void CalibrateEvery5Minutes(void);

int main(void)
{
    DWORD myPacketCounter = 0;
    WORD motion = 0;
    WORD command = 0;
    WORD transmit = 0;

    BoardInit();

    // Initialize radio
    TRX_Reset();
    TRX_Init();
    TRX_PowerLevel(MINUS_6);
    while (TRX_SetState(RX_ON) == 0);
    // Generate a random number to use for the packet counter
    myPacketCounter = ((DWORD)TRX_GetEightRandomBits() << 16) +
                      ((DWORD)TRX_GetEightRandomBits() <<  8) +
                      (TRX_GetEightRandomBits());

    RCON = 0;
    
    while (1)
    {
        ClrWdt();

        // Calibrate radio every 5 minutes
        CalibrateEvery5Minutes();

        if (startupTimer >= STARTUP_SECONDS)
        {
            if (DETECT_US_PIN)
            {
                LED_GREEN = 1;
                motion = 1;
                LEDoffCounter = 0;
            }

            if (DETECT_PIR_PIN)
            {
                LED_RED = 1;
                motion = 1;
                LEDoffCounter = 0;
            }
        }

        if (LEDoffCounter > 1)
        {
            LED_GREEN = 0;
            LED_RED = 0;
            LEDoffCounter = 0;
        }

        if (heartBeatTimer >= HEARTBEAT_MAX_SECONDS)
        {
            transmit = 1;
            command = motion;
        }

        if (motion > 0)
        {
            if (motionTimer >= NO_MOTION_MAX_SECONDS)
            {
                transmit = NUM_OCC_TX;
                command = 65;               // set to occ command
                motionTimer = 0;
            }
            else if (motionTimer > 0)
                motionTimer = 0;
        }

        while (transmit > 0)
        {
            LED_RED ^= 1;                   // toggle current state
            myPacketCounter++;

            TRX_ClearFrame();               // clear TX frame
            TRX_WriteData(myNodeID >> 24);
            TRX_WriteData(myNodeID >> 16);
            TRX_WriteData(myNodeID >> 8);
            TRX_WriteData(myNodeID);
            TRX_WriteData(myPacketCounter >> 16);
            TRX_WriteData(myPacketCounter >> 8);
            TRX_WriteData(myPacketCounter);
            TRX_WriteData(0x00);
            TRX_WriteData(0x00);
            TRX_WriteData(command);

            TRX_TxPacket();                 // send frame data
            TRX_ChangeChannel(CHAN_921);    // change channel
            DelayMs(25);                    // wait for current radio traffic to subside before attempting to TX
            TRX_TxPacket();                 // send frame data
            TRX_ChangeChannel(CHAN_909);    // change channel back

            motion = 0;
            heartBeatTimer = 0;

            LED_RED ^= 1;                   // toggle current state

            if (--transmit > 0)
                DelayMs(OCC_PKT_DELAY_MILLI_SECS); // wait before sending again
        }
    }
}

//*********************************************************************
// timer1 interrupt routine
//*********************************************************************
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0;
    heartBeatTimer++;
    LEDoffCounter++;
    radioCalTimer++;

    if (motionTimer < NO_MOTION_MAX_SECONDS)
        motionTimer++;

    if (startupTimer < STARTUP_SECONDS)
        startupTimer++;
}

void CalibrateEvery5Minutes()
{
    if (radioCalTimer >= RADIO_CAL_SECONDS)             // calibrate radios when limit is reached
    {
        radioCalTimer = 0;
        if (!TRX_Calibrate())
            Reset();                                    // reset micro and radio
    }
}
