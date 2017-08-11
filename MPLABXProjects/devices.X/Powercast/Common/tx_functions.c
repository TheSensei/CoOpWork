/*********************************************************************
 * tx_functions.c
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/


#include <stdlib.h>
#include "xc.h"
#include "SmartConnector.h"
#include "RadioUtils.h"
#include "diag_functions.h"
#include "tx_functions.h"


/*****************************************************************************
  Function:
    void inline txRespPrep(WORD delayInMs)

  Description:
    Does Transmit Response Preparation work

  Parameters:
    delayInMs - suggest min 10ms, max 50 ms

  Returns:
    None
  ***************************************************************************/
void inline txRespPrep(WORD delayInMs)
{
    srand(TMR1);  // randomize delay before sending packet
    rand_int = delayInMs; //10 + (rand() % 40);

    // validate length of request
    myPacketCounter = MODU24_ADD(myPacketCounter, 1u);
        
    TX_delay[TX_write_index] = rand_int;
}


/*****************************************************************************
  Function:
    void inline loadRespHeaderIntoTxBuff(int respCommand, BYTE respSize)

  Description:
    Loads response header into next Tx buffer (copied to rx buffer later)

  Parameters:
    respCommand = response command byte
    respSize = response size in bytes

  Returns:
    None
  ***************************************************************************/
void inline loadRespHeaderIntoTxBuff(int respCommand, BYTE respSize)
{
    TX_buffer[TX_write_index][0] = respSize;
    TX_buffer[TX_write_index][1] = myNodeID >> 24;
    TX_buffer[TX_write_index][2] = myNodeID >> 16;
    TX_buffer[TX_write_index][3] = myNodeID >> 8;
    TX_buffer[TX_write_index][4] = myNodeID;
    TX_buffer[TX_write_index][5] = myPacketCounter >> 16;
    TX_buffer[TX_write_index][6] = myPacketCounter >> 8;
    TX_buffer[TX_write_index][7] = myPacketCounter;
    TX_buffer[TX_write_index][8] = 0x00;  // 3 command bytes
    TX_buffer[TX_write_index][9] = respCommand >> 8;
    TX_buffer[TX_write_index][10] = respCommand;    

    TX_ttl[TX_write_index] = 6;
    TX_rssi[TX_write_index] = RSSI_BASE_VAL;
}


/*****************************************************************************
  Function:
    void inline setTxBuffReady(void)

  Description:
    Adjust Tx write index to buffer can be sent

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void inline setTxBuffReady(void)
{
    TX_write_index++;
    if( TX_write_index == MAX_TX_BUFFERS )
      TX_write_index = 0;

    if( TX_read_index == TX_write_index )
    {
      TX_read_index++;
      countError(ERR_DROP_OLDEST_TX_BUFFER);
      if( TX_read_index == MAX_TX_BUFFERS )
        TX_read_index = 0;
    }

    countError(TX_COUNT);
}
