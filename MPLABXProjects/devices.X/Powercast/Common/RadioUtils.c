/*********************************************************************
 * RadioUtils.c
 * Written by Joseph Ates
 * Copyright 2014
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/
#include "RadioUtils.h"

#include <stdbool.h>
#include <stdint.h>
#include "diag_functions.h"

bool RadioUtils_IsValidRxPacketCount(uint32_t pktCt, uint32_t recPktCt)
{
    bool bRxPckCtValid = !(MODU24_GT(MODU24_ADD(recPktCt,WINDOW_SIZE+1ul), pktCt) &&
            MODU24_LT(recPktCt, MODU24_ADD(pktCt, 1ul)));
    if (!bRxPckCtValid)
        countError(ERR_RX_PCK_COUNT_INVALID);
    return (bRxPckCtValid);
}

bool RadioUtils_IsNotNearTxPacketCount(uint32_t pktCt, uint32_t recPktCt) {
    bool bRxPckCtNotNearTx =  !(MODU24_GT(MODU24_ADD(recPktCt,WINDOW_SIZE+1ul), pktCt) &&
            MODU24_LT(recPktCt, MODU24_ADD(pktCt, WINDOW_SIZE+1ul)));
    if (!bRxPckCtNotNearTx)
        countError(ERR_RX_PCK_COUNT_NEAR_TX);
    return (bRxPckCtNotNearTx);
}
