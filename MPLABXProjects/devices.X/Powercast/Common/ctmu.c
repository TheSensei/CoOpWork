/*********************************************************************
 * ctmu.c
 * Written by Daniel W Harrist
 * Copyright 2015
 ********************************************************************/
#include "ctmu.h"


/****************************************************************************
  Section: Cap sense static variables
  ***************************************************************************/
int currRawData[NUM_CTMU_CH];	// CTMU channel value
int prevRawData[NUM_CTMU_CH];	// previous CTMU channel value
int averageData[NUM_CTMU_CH];	// weighted average of CTMU channel values
int diffRawData[NUM_CTMU_CH];	// difference of avg and cur data
int sensitivity[NUM_CTMU_CH];   // sensitivity of each channel
int startupCount = 0;

/****************************************************************************
  Section: Cap sense functions
  ***************************************************************************/

/****************************************************************************
  Function:
    void InitCapSense(void)

  Summary:
    Initialize arrays.

  Description:
    Initialize arrays.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.
  ***************************************************************************/
void InitCapSense(void)
{
    BYTE i;
    
    // initialize arrays
    for (i = 0; i < NUM_CTMU_CH; i++)
    {
        currRawData[i] = 0;
        diffRawData[i] = 0;
        prevRawData[i] = 0;
        averageData[i] = 0;

        if (i == 0)         sensitivity[i] = 0;
        else if (i == 1)    sensitivity[i] = CHAN1_SENS;
        else if (i == 2)    sensitivity[i] = CHAN2_SENS;
        else if (i == 3)    sensitivity[i] = CHAN3_SENS;
        else if (i == 4)    sensitivity[i] = CHAN4_SENS;
    }


}


/****************************************************************************
  Function:
    void ReadCTMU(int i)

  Summary:
    Samples all CTMU channel.

  Description:
    Samples all CTMU channel and returns the channel that has changed the most.

  Precondition:
    InitCapSense()

  Parameters:
    None.

  Returns:
    Channel that changed the most (0 for no changes).
  ***************************************************************************/
int ReadCTMU()
{
    int curLevel = 0, maxDiff = 0;
    BYTE i, sample;
    
    _CTMUMD = 0;        // enable CTMU
    _ADC1MD = 0;        // enable ADC

    CTMUCON = 0;
    CTMUICON = 0;
    _EDG2SEL = 0x3;     // edge2 src = CTED1
    _EDG1POL = 1;       // edge1 postive
    _EDG1SEL = 0x3;     // edge1 src = CTED1
    _IRNG = 2;          // 5.5uA
	
    AD1CON1 = 0x8000;   // adc on
    AD1CON3 = 0x0000;
    _CTMUEN = 1;        // CTMU on

    if (startupCount < STARTUP_COUNT)
        startupCount++;

    for (i = 0; i < NUM_CTMU_CH; i++)
    {
        // Ground selected CTMU channel
        GroundChannel(i);

        // start sampling
        currRawData[i] = 0;
        for (sample = 0; sample < NUM_SAMPLES; sample++)
        {
            // Drain any internal ADC charge
            _IDISSEN = 1;
            Nop(); Nop(); Nop(); Nop(); Nop();
            _IDISSEN = 0;

            _AD1IF = 0;
            _SAMP = 1;      // Manually start the conversion

            // Charge the capacitive sensor
            _EDG2STAT = 0;  // Make sure edge2 is 0
            _EDG1STAT = 1;  // Set edge1 - Start Charge

            // Charge Delay
            Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
            Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();

            _EDG1STAT = 0;  // Clear edge1 - Stop Charge

            _SAMP = 0;      // Manually Initiate an ADC conversion
            while(!_AD1IF); // Wait for the A/D conversion to finish

            currRawData[i] += ADC1BUF0;     // add current sample to total reading

            _SAMP = 0;      // Clear ADC status
            _AD1IF = 0;
            _DONE = 0;
        }

        // Discharge channel
        DischargeChannel(i);

        //  to previous data
        prevRawData[i] = averageData[i];	// update previous value
        diffRawData[i] = averageData[i] - currRawData[i];	// get difference
	
       	// On power-up, reach steady-state readings
        // During power-up, system must be Initialized, Set average ea. pass = raw data
        if (startupCount < STARTUP_COUNT)
            averageData[i] = currRawData[i];	// save last startup read
        else if (currRawData[i] > averageData[i])
            averageData[i] = currRawData[i];	// adjust average to new high
        else
        {
            // Average in the new value
            // Average (all buttons) each NUM_AVG times through the algorithm
            averageData[i] = ((currRawData[i] - prevRawData[i]) / CAP_NUM_AVG) + prevRawData[i];
        }
    }	
			
    _CTMUMD = 1;    // CTMU & ADC off to save power
    _ADC1MD = 1;

    // Calculate if and where the switch was pressed
    for (i = 1; i < NUM_CTMU_CH; i++)           // skip dummy channel 0
    {
        if ((diffRawData[i] - sensitivity[i]) > maxDiff)
        {
            maxDiff = diffRawData[i] - sensitivity[i];
            curLevel = i;
        }
    }

    return curLevel;
    
} //end ReadCTMU()

void GroundChannel(int channel)
{
    switch (channel)
    {
        case 0:
            CHAN0_AN = 0;
            AD1CHS = CHAN0_CHANNEL;
            CHAN0_TRIS = 0;
            Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
            CHAN0_TRIS = 1;
            break;

        case 1:
            CHAN1_AN = 0;
            AD1CHS = CHAN1_CHANNEL;
            CHAN1_TRIS = 0;
            Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
            CHAN1_TRIS = 1;
            break;

        case 2:
            CHAN2_AN = 0;
            AD1CHS = CHAN2_CHANNEL;
            CHAN2_TRIS = 0;
            Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
            CHAN2_TRIS = 1;
            break;

        case 3:
            CHAN3_AN = 0;
            AD1CHS = CHAN3_CHANNEL;
            CHAN3_TRIS = 0;
            Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
            CHAN3_TRIS = 1;
            break;

        case 4:
            CHAN4_AN = 0;
            AD1CHS = CHAN4_CHANNEL;
            CHAN4_TRIS = 0;
            Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
            CHAN4_TRIS = 1;
            break;

        default:
            break;
    }
}

void DischargeChannel(int channel)
{
    switch (channel)
    {
        case 0:
            CHAN0_AN = 1;
            CHAN0_TRIS = 0;
            break;

        case 1:
            CHAN1_AN = 1;
            CHAN1_TRIS = 0;
            break;

        case 2:
            CHAN2_AN = 1;
            CHAN2_TRIS = 0;
            break;

        case 3:
            CHAN3_AN = 1;
            CHAN3_TRIS = 0;
            break;

        case 4:
            CHAN4_AN = 1;
            CHAN4_TRIS = 0;
            break;

        default:
            break;
    }
}
