/*********************************************************************
 * ctmu.h
 * Written by Daniel W Harrist
 * Copyright 2015
 ********************************************************************/
#ifndef _CTMU_H
#define _CTMU_H

#include "HardwareProfile.h"    // should include NUM_CTMU_CH and NUM_CTMU_SAMPS_PER_CHAN

/****************************************************************************
  Section: CTMU Defines
  ***************************************************************************/
#define CAP_NUM_AVG         8   // weight of averaging
#define STARTUP_COUNT       40  // initial readings for averaging starts
#define NUM_SAMPLES         NUM_CTMU_SAMPS_PER_CHAN   // samples per each channel

/****************************************************************************
  Section: CTMU Function Prototypes
  ***************************************************************************/
void InitCapSense(void);
int ReadCTMU();
void GroundChannel(int);
void DischargeChannel(int);

#endif
