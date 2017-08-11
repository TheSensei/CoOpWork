/*********************************************************************
 * diag_functions.c
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/


#include "xc.h"
#include "GenericTypeDefs.h"
#include "AT86RF212.h"
#include "fw_update_req_handlers.h"
#include "SmartConnector.h"
#include "tx_functions.h"
#include "diag_functions.h"

WORD diagCounts[TOTAL_DIAGNOSTIC_COUNTS];  // 16-bit elements holding error counts (0x0000-0xFFFF) - one element for each err type
BYTE diagBitMap[BYTES_IN_DIAG_BIT_MAP];   // bit set in map means see non-zero count in diagCounts


// function prototypes
void getGwBitMapFromRx (bool * bGwBitMapZero, BYTE *gwBitMap);
void setGwBitMapToAllOnes(BYTE *gwBitMap);
void clearUnusedGwBitMapBits(BYTE *gwBitMap);
void prepDiagRespForGw(BYTE gwFlags, BYTE gwRespReq, bool bZeroCountsInDiagResp, BYTE *gwBitMap, BYTE *lastRespCountBitMap);
BYTE addCountsToDiagResp(bool bZeroCountsInDiagResp, BYTE *gwBitMap, BYTE *lastRespCountBitMap, int txBuffIndex);

#if USE_DIAG_TEST_CODE_AT_INIT_TIME
void insertErrorsForTesting(void);
#endif


/*****************************************************************************
  Function:
    void countRadioErr(int radioErrType)

  Description:
    Increments appropriate error count that can be sent to GW in Diagnostic Response

  Parameters:
    radioErrType - indicate type of error - see AT86RF212.h

  Returns:
    None
  ***************************************************************************/
void countRadioErr(int radioErrType)
{
    switch (radioErrType)
    {
        case TRX_NO_ERROR:
            // no statistic to increment at this time
            break;

        case TRX_BUFFER_OVERUN:
            countError(ERR_TRX_BUFFER_OVERRUN);
            break;

        case TRX_MISSED_IRQ:
            countError(ERR_TRX_MISSED_IRQ);
            break;

        case TRX_FAILED_CAL:
            countError(ERR_TRX_FAILED_CAL);
            break;

        case TRX_FAILED_STATE_CHANGE:
            countError(ERR_TRX_FAILED_STATE_CHANGE);
            break;

        case TRX_FAILED_TX:
            countError(ERR_TRX_FAILED_TX);
            break;

        case TRX_FAILED_TX_TIMEOUT:
            countError(ERR_TRX_TX_TIMEOUT);
            break;

        case TRX_RX_CRC_ERR:
            countError(ERR_TRX_RX_CRC);
            break;

        case TRX_DROP_NEWEST_APP_RX_BUF:
            countError(ERR_TRX_DROP_NEW_RX_BUF);
            break;

        case TRX_DROP_OLDEST_APP_RX_BUF:
            countError(ERR_TRX_DROP_OLDEST_RX_BUF);
            break;

        case TRX_RX_FRAME_TOO_LARGE:
            countError(ERR_TRX_RX_FRAME_TOO_LARGE);
            break;

        default:
            // Someone may have added errors to AT86RF212.h but didn't update this fnc
            countError(ERR_TRX_UNKNOWN);
            break;
   }
}


/*****************************************************************************
  Function:
    WORD countError(DIAG err)

  Description:
    Increments appropriate error count that can be sent to GW in Diagnostic Response

   Parameters:
    DIAG err - enum for each error type

  Returns:
    current count of this error type
  ***************************************************************************/
WORD countError(DIAG err)
{
    int diagNumber = (int) err;
    int bitMapIndex = diagNumber/8;
    int diagNumberForIndex = diagNumber % 8;

    // Don't let error counts wrap to zero
    if (diagCounts[diagNumber] == 0xFFFF)
        return 0;

    // increment error count
    diagCounts[diagNumber]++;

    // Make a bit mask for this err number (max 64 bits, or errors)
    BYTE diagBitInIndex = (1 << diagNumberForIndex);  // 0x1 (0), 0x2 (1), 0x4 (2), ... 0x80 (7))

    // Make sure bit for this error type is set (if bit set, means look in diagCounts to see non-zero count of this error)
    diagBitMap[bitMapIndex] |= diagBitInIndex;

    return(diagCounts[diagNumber]);  // return current count of this error
}


/*****************************************************************************
  Function:
    void clearError(DIAG err)

  Description:
    Clears appropriate error count

  Parameters:
    DIAG err - enum for each error type

  Returns:
    None
  ***************************************************************************/
void clearError(DIAG err)
{
    int diagNumber = (int) err;
    int bitMapIndex = diagNumber/8;
    int diagNumberForIndex = diagNumber % 8;

    // clear error count
    diagCounts[diagNumber] = 0;

    // Make a bit mask for this err number (max 64 bits, or errors)
    BYTE diagBitInIndex = (1 << diagNumberForIndex);  // 0x1 (0), 0x2 (1), 0x4 (2), ... 0x80 (7))

    // Make sure bit for this error type is clear
    diagBitMap[bitMapIndex] &= ~diagBitInIndex;
}


/*****************************************************************************
  Function:
   void clearErrors(QWORD errTypeClearBitMap)

  Description:
    Clears errors specified in bitmap argument only

  Parameters:
    QWORD errTypeClearBitMap - a 1 bit means to clear

  Returns:
    None
  ***************************************************************************/
void clearErrors(BYTE * clearBitMap)
{
    int ii = 0;
    int jj = 0;
    int diagNumber = 0;
    
    // Clear errors for bits set in the clear bit map
    for (ii=0; ii < BYTES_IN_DIAG_BIT_MAP; ii++)
    {
        for (jj = 0;  jj < 8;  jj++)
        {
            diagNumber = (ii * 8) + jj;
            // if next bit set in clear bitmap - clear the error
            if (clearBitMap[ii] & (1 << jj))
                clearError(diagNumber);
        }
    }    
}


/*****************************************************************************
  Function:
    void clearAllErrors(void)

  Description:
    Clear all diagnostics

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void clearAllErrors(void)
{
    int ii = 0;

    // Clear bit map
    for (ii=0; ii < BYTES_IN_DIAG_BIT_MAP; ii++)
        diagBitMap[ii] = 0;
    
    // Clear all diag counts
    for (ii=0; ii < TOTAL_DIAGNOSTIC_COUNTS; ii++)
        diagCounts[ii] = 0;
}

BYTE
respDiag_length( BYTE *respDiag )
{
  BYTE i;
  BYTE length = 12; // for version 1
  BYTE gwVersion = respDiag[0];
  BYTE gwFlags = respDiag[1];
  BYTE gwRespReq = gwFlags & REQ_DIAG_FLAGS_GW_RESP_REQ_BITS;

  // this must be accounted for when it changes
  if( gwVersion != DIAG_FORMAT_VER )
    return length;

  if( gwFlags & REQ_DIAG_FLAGS_GW_BITMAP_PRESENT_BIT ||
      gwRespReq == DIAG_GW_RESP_REQ_BITMAP ||
      gwRespReq == DIAG_GW_RESP_REQ_BITMAP_AND_COUNTS )
    length += BYTES_IN_DIAG_BIT_MAP;
  if( gwRespReq == DIAG_GW_RESP_REQ_COUNTS ||
      gwRespReq == DIAG_GW_RESP_REQ_BITMAP_AND_COUNTS )
    for( i = 0; i < TOTAL_DIAGNOSTIC_COUNTS && length + 2 < CONNECTOR_TX_FRAME; i++ )
      if( respDiag[2 + i / 8] & (1 << (i % 8)) )
        length += 2;
  return length;
}

/*****************************************************************************
  Function:
   void reqDiag_handler(void)

  Description:
    Process GW request for Diagnostic Operation to this Node (P2P)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqDiag_handler(void)
{
    BYTE gwBitMap[BYTES_IN_DIAG_BIT_MAP];
    bool gwBitMapProvided = false;
    static BYTE lastRespCountBitMap[BYTES_IN_DIAG_BIT_MAP];  // static saved between fnc calls
    BYTE gwFlags = RX_packet[11];
    BYTE gwClearReq = gwFlags & REQ_DIAG_FLAGS_GW_CLEAR_REQ_BITS;
    BYTE gwRespReq = gwFlags & REQ_DIAG_FLAGS_GW_RESP_REQ_BITS;
    bool bZeroCountsInDiagResp = true;
    
    if ((gwFlags & REQ_DIAG_FLAGS_RESERVED_BITS) != 0x00)
    {
        // if reserved bits set - err
        countError(ERR_DIAG_FLAGS);
        return;
    }

    if ((gwClearReq == DIAG_GW_CLEAR_REQ_NONE) &&
        (gwRespReq == DIAG_GW_RESP_REQ_NONE))
    {
        // If GW didn't request any work to be done -err
        countError(ERR_DIAG_FLAGS);
        return;
    }

    if (gwFlags & REQ_DIAG_FLAGS_GW_BITMAP_PRESENT_BIT)
    {
        if ((gwRespReq == DIAG_GW_RESP_REQ_BITMAP) ||
            (gwRespReq == DIAG_GW_RESP_REQ_BITMAP_AND_COUNTS))
        {
            // If GW provided bitmap and requested Node's bitmap too - invalid req
            countError(ERR_DIAG_FLAGS);
            return;
        }
        
        bool bGwBitMapZero = true;
        getGwBitMapFromRx(&bGwBitMapZero, gwBitMap);

        if (bGwBitMapZero)
        {
            countError(ERR_DIAG_GW_BITMAP_ZERO);
            return;
        }
        gwBitMapProvided = true;
    }
    // else short Diag Req (no GW bitmap provided))
    else
    {
        if ((gwClearReq == DIAG_GW_CLEAR_REQ_TO_BE_REPORTED) &&
            (gwRespReq == DIAG_GW_RESP_REQ_NONE))
        {
            // If no response can't clear diags in response - invalid req
            countError(ERR_DIAG_FLAGS);
            return;
        }

        // If GW doesn't provide bitmap, GW wants default map of all one bits (e.g. all diags)
        setGwBitMapToAllOnes(gwBitMap);
        if (gwRespReq == DIAG_GW_RESP_REQ_BITMAP_AND_COUNTS)
            bZeroCountsInDiagResp = false;
    }

    // Clear GW bitmap (from GW or default value) of unused error bits in last array index
    clearUnusedGwBitMapBits(gwBitMap);

    // There are four types of GW-Clear_Requests, this one is done "pre" create response buffer
    if (gwClearReq == DIAG_GW_CLEAR_REQ_LAST_REPORTED)
       clearErrors(lastRespCountBitMap);  // clear those diags in last/previous Diag Resp

#if USE_DIAG_TEST_CODE_AT_INIT_TIME
    insertErrorsForTesting();
#endif

    // Create response unless GW requested "no response"
    if (gwRespReq != DIAG_GW_RESP_REQ_NONE)
        prepDiagRespForGw(gwFlags, gwRespReq, bZeroCountsInDiagResp, gwBitMap, lastRespCountBitMap);

    // Check for "post" create buffer Clear requests
    if (gwClearReq == DIAG_GW_CLEAR_REQ_TO_BE_REPORTED)
    {
        if(gwBitMapProvided)
            clearErrors(gwBitMap);  // clear diags in GW bit map
        else
            clearErrors(lastRespCountBitMap);  // clear diag counts that will be reported in GW response just created above
    }
    else if (gwClearReq == DIAG_GW_CLEAR_REQ_ALL)
        clearAllErrors();
}


/*****************************************************************************
  Function:
   void getGwBitMapFromRx (bool * bGwBitMapZero, BYTE *gwBitMap)

  Description:
   Copies GW Bit Map from Diag req to the gwBitMap array

  Parameters:
    bGwBitMapZero - pointer to bool, set by fnc if all bits zero
    gwBitMap - pointer to array of diags GW specified in Diag req

  Returns:
    None
  ***************************************************************************/
void getGwBitMapFromRx (bool *bGwBitMapZero, BYTE *gwBitMap)
{
    int ii = 12;  // starting offset of GW bitmap
    int jj = 0;

    // Get GW bitmap from Rx buffer
    for (jj = BYTES_IN_DIAG_BIT_MAP - 1;  jj >= 0; jj--)
    {
        gwBitMap[jj] = RX_packet[ii++];
        if (gwBitMap[jj] != 0)
           *bGwBitMapZero = false;
    }
}


/*****************************************************************************
  Function:
   void setGwBitMapToAllOnes(BYTE *gwBitMap)

  Description:
    Sets gwBitMap to all one bits (e.g., process all diags)

  Parameters:
    gwBitMap - pointer to array of diags GW specified in Diag req

  Returns:
    None
  ***************************************************************************/
void setGwBitMapToAllOnes(BYTE *gwBitMap)
{
    int jj = 0;

    for (jj = BYTES_IN_DIAG_BIT_MAP - 1;  jj >= 0; jj--)
    {
        gwBitMap[jj] = 0xFF;
    }
}


/*****************************************************************************
  Function:
   void clearUnusedGwBitMapBits(BYTE *gwBitMap)

  Description:
    The last byte of the gwBitMap array may contains some unused bits that are cleared.

  Parameters:
    gwBitMap - pointer to array of diags GW specified in Diag req

  Returns:
    None
  ***************************************************************************/
void clearUnusedGwBitMapBits(BYTE *gwBitMap)
{
    if ((TOTAL_DIAGNOSTIC_COUNTS % 8) != 0)
    {
        // The last index in gwBitMap has 1 or more bits that are unused, so clear unused bits
        int unusedDiagNumberForLastIndex = TOTAL_DIAGNOSTIC_COUNTS % 8;
        BYTE clearNextBitMask = (1 << unusedDiagNumberForLastIndex);
        for (;;)
        {
            gwBitMap[BYTES_IN_DIAG_BIT_MAP - 1] &= ~clearNextBitMask;  // clear next unused bit
            if (clearNextBitMask == 0x80)
                break;
            else
                clearNextBitMask = clearNextBitMask << 1;
        }
    }
}


/*****************************************************************************
  Function:
    void prepDiagRespForGw(BYTE gwFlags, BYTE gwRespReq, bool bZeroCountsInDiagResp, BYTE *gwBitMap, BYTE *lastRespCountBitMap)

  Description:
    Prepare a buffer with the Diag Response data to be sent to the GW

  Parameters:
    gwFlags - from GW Diag req and contains additional info about GW req
    gwRespReq - from GW Diag req and indicates type of response GW wants
    bZeroCountsInDiagResp - true means okay to put zero counts in resp
    gwBitMap - pointer to array of diags GW specified in Diag req
    lastRespCountBitMap - point to bitmap array for diag counts in Diag resp sent to GW

  Returns:
    None
  ***************************************************************************/
void prepDiagRespForGw(BYTE gwFlags, BYTE gwRespReq, bool bZeroCountsInDiagResp, BYTE *gwBitMap, BYTE *lastRespCountBitMap)
{
    int ii = 0;
    int jj = 0;

    txRespPrep(5);

    ii = 1 + 10;  // Tx buff offset past 10-bytes of header info - load header near end of fnc when know resp size

    TX_buffer[TX_write_index][ii++] = DIAG_FORMAT_VER;  // Tell GW format version - maybe a mix of nodes using different formats

    TX_buffer[TX_write_index][ii++] = gwFlags;  // echo back GW Flag field rxed

    // If response should contain GW's bitmap - add adjusted GW bit map (bits for unused positions cleared)
    if (gwFlags & REQ_DIAG_FLAGS_GW_BITMAP_PRESENT_BIT)
    {
        // Store adjusted GW bit map in Resp buffer - high order bytes 1st
        for (jj = BYTES_IN_DIAG_BIT_MAP - 1;  jj >= 0; jj--)
            TX_buffer[TX_write_index][ii++] = gwBitMap[jj];
    }
    // Else, check if response should contain Node's bitmap
    else if ((gwRespReq == DIAG_GW_RESP_REQ_BITMAP) ||
        (gwRespReq == DIAG_GW_RESP_REQ_BITMAP_AND_COUNTS))  // these options caught as err above if GW bitmap provided
    {
        // Store Node's Diag bit map in resp buffer - high order bytes 1st
        for (jj = BYTES_IN_DIAG_BIT_MAP - 1;  jj >= 0; jj--)
            TX_buffer[TX_write_index][ii++] = diagBitMap[jj];
    }

    BYTE respSize = ii - 1;

    // Check if response should contain diag counts
    if ((gwRespReq == DIAG_GW_RESP_REQ_COUNTS) || (gwRespReq == DIAG_GW_RESP_REQ_BITMAP_AND_COUNTS))   // this 2nd option caught as err above if GW bitmap provided
        respSize = addCountsToDiagResp(bZeroCountsInDiagResp, gwBitMap, lastRespCountBitMap, ii /* next offset in Tx buffer */);

    // Load Resp header near end of fnc because need to know size and it may vary
    loadRespHeaderIntoTxBuff(RESP_DIAG_OP, respSize);

    BYTE wiBit = 1 << (TX_write_index & 7);
    TX_low_power[TX_write_index / 8] &= ~wiBit;

    setTxBuffReady();  // tell radio driver okay to tx this resp to GW
}


/*****************************************************************************
  Function:
   BYTE addCountsToDiagResp(bool bZeroCountsInDiagResp, BYTE *gwBitMap, BYTE *lastRespCountBitMap, int txBuffIndex)

  Description:
    Add any diagnostic counts to buffer based on requested diags per gwBitMap

  Parameters:
    bZeroCountsInDiagResp - true means okay to put zero counts in resp
    gwBitMap - pointer to array of diags GW specified in Diag req
    lastRespCountBitMap - point to bitmap array for diag counts in Diag resp sent to GW
    txBuffIndex - index of next byte in tx buffer where 1st count is to be stored

  Returns:
    size of response
  ***************************************************************************/
BYTE addCountsToDiagResp(bool bZeroCountsInDiagResp, BYTE *gwBitMap, BYTE *lastRespCountBitMap, int txBuffIndex)
{
    int jj = 0;
    int ii = txBuffIndex;

    // clear before set bits in this map below
    for (jj=0; jj < BYTES_IN_DIAG_BIT_MAP; jj++)
        lastRespCountBitMap[jj] = 0;

    // Store counts in same order as bitmaps - higher numbers first
    for (jj = TOTAL_DIAGNOSTIC_COUNTS - 1; jj >= 0; jj--)
    {
        int bitMapIndex = jj/8;
        int diagNumberForIndex = jj % 8;
        BYTE diagBitInIndex = (1 << diagNumberForIndex);  // create next bit mask in high to low order (xx-0))

        // If GW bitmap requested this diag count (or doing all diag counts and defaulted gw bimap to all 1's)
        if ((gwBitMap[bitMapIndex] & diagBitInIndex) != 0)
        {
            // Save count in Diag resp
            WORD count = diagCounts[jj];
            
            if (count == 0 && !bZeroCountsInDiagResp)
                    continue;  // don't process counts of zero

            // Save copy of 2-byte  in resp whether count zero or not
            TX_buffer[TX_write_index][ii++] = (count & 0xFF00) >> 8;
            TX_buffer[TX_write_index][ii++] = (count & 0x00FF);

            // Adjust bitmap of counts in last resp
            lastRespCountBitMap[bitMapIndex] |= diagBitInIndex;

            // Check exit "for" loop early
            // Don't overwrite past end of radio driver tx frame buffer where tailer has to be stored with next diag count.
            // GW can issue another Diag req with a gw bitmap representing any missing diags if counts truncated
            if ((ii + 1) > CONNECTOR_TX_FRAME)  // use +1 because ii++ above adds one byte for 1st half of next count, need to add one more for 2nd byte next count
                break;
        }
    }
    return (ii - 1);  // return size of response
}



#if USE_DIAG_TEST_CODE_AT_INIT_TIME
void test_code_load_counts_with_err_number(void)
{
    int ii = 0;

    //set bits in bitmap for errors used at time wrote this test fnc
    diagBitMap[0] = 0xFF;
    diagBitMap[1] = 0xFF;
    diagBitMap[2] = 0xFF;
    diagBitMap[3] = 0xFF;
    diagBitMap[4] = 0xFF;
    diagBitMap[5] = 0xFF;
    
    // Load counts with their error number plus 1
    for (ii = 0; ii < TOTAL_DIAGNOSTIC_COUNTS; ii++)
    {
        diagCounts[ii] = (ii+1);
    }
}


void insertErrorsForTesting(void)
{
    // Add some errors for testing
    countError(ERR_REQ_SIZE_INVALID);          // first err in enum list
    countError(ERR_FW_DATA_PAGE_ID_BIT6_ZERO); // 10th err
    countError(ERR_TRX_UNKNOWN);               // last err
}
#endif
