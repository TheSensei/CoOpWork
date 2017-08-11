/*********************************************************************
 * main.c
 * Written by Daniel W Harrist
 * Copyright 2016
 ********************************************************************/
#include "HardwareProfile.h"
#include "AT86RF212.h"

//************************** FUNCTIONS ************************************
void Power(int);
WORD ADC_read(int);
void Config_PIR(long);

//************************** CONSTANTS ************************************
// Sensor ADC inputs
#define REF_CHANNEL     10
#define BATT_CHANNEL    5

// Values for calculations
#define REF_VAL         1280000UL       // Reference voltage 1250mV * 1024
#define LOW_BATT        3100
#define TIMER_MAX       480             // 4 minutes between heartbeat packets
#define MOTION_MAX      720             // 6 minutes of vacancy before sending another TURN_ON command
#define STARTUP_MAX     90              // 45 seconds before reading sensor
#define NUM_OCC_TX      2               // number of 65 packets

//************************** VARIABLES ************************************
// flags set by interrupt
WORD buttonPressed = 0;
WORD motionSensed = 0;

__prog__ const volatile DWORD NodeID __attribute__((space(prog), address(0x2bf0)));
//DWORD NodeID = 0x050000e1;

#define BUTTON                    

/*********************************************************************
* int main(void)
**********************************************************************/
int main(void)
{   
    WORD ADC_res = 0;
    WORD vdd_mv = 0;
    SHORT batt = 0;
    WORD batt_mv = 0;
    WORD motion = 0;
    WORD send_timer = TIMER_MAX;
    WORD motion_timer = MOTION_MAX;
    WORD startup_count = 0;
    DWORD packet_count = 0;
    BYTE transmit = 0;
    
    // Initialize the system
    BoardInit();
    Config_PIR(0x05C2B10);          // Bandpass filter, Wake up mode, 8s window, 2x pulse counter, 0.5s blind time, 1001mV threshold

#ifdef BUTTON
    if (_WDTO == 0) {               // if WDT was not cause of reset, wait for button press
        while (!buttonPressed)          // wait for button press flag
            Sleep();
    }
#endif
    _WDTO = 0;
    
    Power(POWER_UP);
    DelayMs(3);
    TRX_Init();
    while (TRX_SetState(RX_ON) == 0);
    // Generate a random number to use for the packet counter
    packet_count = ((DWORD)TRX_GetEightRandomBits() << 16) +
                   ((DWORD)TRX_GetEightRandomBits() <<  8) +
                   (TRX_GetEightRandomBits());
    Power(POWER_DOWN);
    
    Sleep();

    // start continuous loop
    while (1)
    {
        if (buttonPressed)
            buttonPressed = 0;
        else
            send_timer++;
        
        if (startup_count < STARTUP_MAX)
            startup_count++;
        
        if (motionSensed)
        {
            motion = 1;
			
            if (startup_count >= STARTUP_MAX)
            {
                if (motion_timer == MOTION_MAX)
                    motion_timer = 0;   // send packet immediately
                else if (motion_timer > 2)
                    motion_timer = 2;   // reset timer so no packet sent
            }
        }

        if (motion_timer < NUM_OCC_TX)
            transmit = 1;
        else if (send_timer >= TIMER_MAX)
            transmit = 2;

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
                ADC_res = ADC_read(REF_CHANNEL);
                vdd_mv = (WORD)(REF_VAL / (float)ADC_res);

                // Measure battery voltage
                ADC_res = ADC_read(BATT_CHANNEL);
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
                TRX_ChangeChannel(CHAN_921);    // change channel
                DelayMs(25);                    // wait for current radio traffic to subside before attempting to TX
                TRX_TxPacket();                 // send frame data
            }
            
            Power(POWER_DOWN);

            motion = 0;                         // reset flags
            transmit = 0;
            send_timer = 0;
        }

        if (motion_timer < MOTION_MAX)
            motion_timer++;                     // increment read counter to max

        if (motionSensed > 0)
        {
            motionSensed--;                     // multiple sleeps needed before releasing pin
            MOT_CN_PDEN = 1;                    // pull pin low manually
        }
        else
            MOT_CN_PDEN = 0;                    // release pin pull down
        
        Sleep();
    }   // END WHILE

    return 0;                               // should never get here
    
}   // END MAIN

/*****************************************
 * Function prototypes
 *****************************************/
#define ADC_TO	500
WORD ADC_read(int channel)
{
    WORD result = 0;
    int i = 0;
    WORD timeout = 0;

    // ADC read
    AD1CHS = channel;               // set channel to measure
    AD1CON1bits.ADON = 1;           // turn ADC on
    for (i = 0; i < 50; i++);       // startup time
    AD1CON1bits.SAMP = 1;           // start sampling
    while (!AD1CON1bits.DONE){timeout++; if (timeout == ADC_TO) break; }
    result = ADC1BUF0;             // read ADC result
    AD1CON1bits.ADON = 0;           // turn ADC off
    return result;
}

void Config_PIR(long regval)
{
    int i;
    int nextbit;                    // individual config register bits
    long regmask = 0x1000000;       // mask for adjusting register bit to be edited

    SER_IN_TRIS = 0;                // set SERIN pin to output
    SER_IN = 0;

    for (i=0;i<25;i++)
    {
        nextbit = (regval&regmask) != 0;
        regmask >>= 1;              // move mask to edit next bit
        SER_IN = 0;                 // low to high transition
        SER_IN = 1;
        SER_IN = nextbit;           // write bit to register
        Delay10us(10);
        ClrWdt();
    }

    LED = 1;
    DelayMs(100);
    LED = 0;
    DelayMs(100);
    ClrWdt();
    LED = 1;
    DelayMs(100);
    LED = 0;
    DelayMs(100);
    ClrWdt();
    LED = 1;
    DelayMs(100);
    LED = 0;
    DelayMs(100);
    ClrWdt();
    LED = 1;
    DelayMs(100);
    LED = 0;
    DelayMs(100);
    ClrWdt();

    SER_IN = 0;
    DelayMs(1);
//    SER_IN_TRIS = 1;                // set SERIN pin back to input
}

/*********************************************************************
* Function:         void CNInterrupt(void)
**********************************************************************/
void _ISRFAST __attribute__((interrupt, auto_psv)) _CNInterrupt(void)
{
    _CNIF = 0;                  // clear the interrupt flag
    _CNIE = 0;                  // disable interrupt

    if (START_BTN == 1)
    {
        DelayMs(75);            // debounce
        if (START_BTN == 1)     // check again
            buttonPressed = 1;  // set flag for main loop
    }

    if (DIRECT_LINK == 1)
        motionSensed = 2;       // set flag for main loop
    
    _CNIE = 1;                  // re-enable interrupt
}


