/*********************************************************************
 * SmartSwitch - Audacy Devices
 * main.c
 * Written by Daniel Harrist
 * Copyright 2016
 ********************************************************************/
#include "HardwareProfile.h"
#include "../Common/AT86RF212.h"
#include "../Common/ctmu.h"

//************************** VARIABLES ************************************
// Values for calculations
#define REF_VAL         1280000UL       // Reference voltage 1250mV * 1024
#define LOW_BATT        3100
#define HEARTBEAT       1200            // 5 minutes

__prog__ const volatile DWORD NodeID __attribute__((space(prog), address(0x2bf0)));
//DWORD NodeID = 0x04000074;
//DWORD NodeID = 0x0A000091;

// globals
volatile WORD proceed = 0;
volatile BYTE transmit = 0;
volatile WORD heartbeat_counter = HEARTBEAT + 1; // Allows device to heartbeat after initial button press

/*********************************************************************
* int main(void)
**********************************************************************/
int main(void)
{   
    DWORD packet_count = 0;
    WORD ADC_res = 0;
    WORD vdd_mv = 0;
    BYTE batt = 0;
    WORD batt_mv = 0;
    int buttonPressed = 0;
    int command = 0;
    
    // Initialize the system
    BoardInit();      
    
#ifdef HAS_BUTTON
    // sleep in a loop until button pressed
    while (proceed == 0)
        Sleep();
#endif

    LED = 1;			// blink LED at startup
    DelayMs(20);		// startup delay
    LED = 0;

    InitCapSense();

    // start continuous loop
    while (1)
    {
        heartbeat_counter++;
		
        if (transmit == 2) buttonPressed = 0;  // "Start" button was pressed for heartbeat
        else  buttonPressed = ReadCTMU();      // check slider

#if defined ON_OFF_FLUSH
        if ((buttonPressed == 2) || (buttonPressed == 3))   transmit = 1;
#else
        if (buttonPressed)  transmit = 1;
#endif
        else if (heartbeat_counter > HEARTBEAT) transmit = 2;

        if (transmit)
        {
            if (transmit == 1)
                LED = 1;

            transmit = 0;
            heartbeat_counter = 0;

            command = 0;
            if (buttonPressed == 1)             command = COMMAND_ONE;
            else if (buttonPressed == 2)        command = COMMAND_TWO;
            else if (buttonPressed == 3)        command = COMMAND_THREE;
            else if (buttonPressed == 4)        command = COMMAND_FOUR;

            Power(POWER_UP);
            DelayMs(3);                     // radio startup delay
			
            TRX_Init();
            TRX_PowerLevel(NOFEM_POWER);         // maximum output power
			
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
            TRX_WriteData(command);         // command = 0 for heartbeat

            TRX_TxPacket();                 // send frame data
            TRX_ChangeChannel(CONNECTOR_CHAN_2);    // change channel
            TRX_TxPacket();                 // send frame data
			
            DelayMs(6);
			
            TRX_ChangeChannel(CONNECTOR_CHAN_1);    // change channel
            TRX_TxPacket();                 // send frame data
            TRX_ChangeChannel(CONNECTOR_CHAN_2);    // change channel
            TRX_TxPacket();                 // send frame data

            Power(POWER_DOWN);
            LED = 0;
        }
		
        Sleep();                            // wait for WDT
    }   // END WHILE
}   // END MAIN

/*********************************************************************
* ADC Reading function
**********************************************************************/
#define ADC_TO 500
WORD ADC_read(int channel)
{
    WORD ADC_res = 0;
    int i = 0;
    WORD timeout = 0;
			
    // ADC read
    AD1CHS = channel;           // set channel to measure
    _ADON = 1;                  // turn ADC on
    for (i = 0; i < 50; i++);   // startup time
    _SAMP = 1;                  // start sampling
    while (!_DONE){timeout++; if (timeout > ADC_TO) break; }
    ADC_res = ADC1BUF0;         // read ADC result
    _ADON = 0;                  // turn ADC off
    
    return ADC_res;
}

/*********************************************************************
* Change Notification Interrupt Routine
**********************************************************************/
void _ISRFAST __attribute__((interrupt, auto_psv)) _CNInterrupt(void) 
{
    _CNIF = 0;                  // clear the interrupt flag
    //_CNIE = 0;                  // disable interrupt

#ifdef HAS_BUTTON	
    if (START_BTN == 1) {
        proceed = 1;
        if (heartbeat_counter != 0) { // Only delay/transmit when sleeping
            transmit = 2;
            DelayMs(50); // Crude debounce
        }
    }
#else
    transmit = 2;
#endif

//    IEC1bits.CNIE = 1;          // don't enable interrupt
}	
