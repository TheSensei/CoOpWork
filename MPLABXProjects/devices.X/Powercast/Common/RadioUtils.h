/*********************************************************************
 * RadioUtils.h
 * Written by Joseph Ates
 * Copyright 2014
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/
#ifndef RadioUtils_H
#define RadioUtils_H

#include <stdbool.h>
#include <stdint.h>


#define WINDOW_SIZE 99ul

////////////////////////////////////////////////////////////////////////
// The following functions are based off RFC 1982 modulo arithmetic
// It is important to beware of implicit signed conversion in your
// arithmetic (especially with constants). Make sure to postfix constants
// with u to make unsigned.

#define MODU24_ADD(i1, i2) (((i1) + (i2)) % 16777216ul) 
#define MODU24_LT(i1, i2) (((i1) < (i2) && (i2) - (i1) < 8388608ul) || \
                           ((i1) > (i2) && (i1) - (i2) > 8388608ul))

#define MODU24_GT(i1, i2) MODU24_LT(i2, i1)
                           

/**
  Check to see if packet is not within invalid window of recently received packets.
  */

bool RadioUtils_IsValidRxPacketCount(uint32_t currentCount, uint32_t receivedCount);

bool RadioUtils_IsNotNearTxPacketCount(uint32_t currTxCt, uint32_t recPktCt);

#endif // RadioUtils_H
