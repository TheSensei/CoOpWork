/*********************************************************************
 * tx_functions.h
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#ifndef TX_FUNCTIONS_H
#define	TX_FUNCTIONS_H

#include "GenericTypeDefs.h"

void inline txRespPrep(WORD delayInMs);
void inline loadRespHeaderIntoTxBuff(int respCommand, BYTE respSize);
void inline setTxBuffReady(void);

#endif	/* TX_FUNCTIONS_H */

