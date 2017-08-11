/*********************************************************************
 * fw_update_req_handlers.c
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#include "HardwareProfile.h"
#include <stdlib.h>
#include "RadioUtils.h"
#include "AT86RF212.h"
#include "version.h"
#include "SmartConnector.h"
#include "../Common/md5_functions.h"
#include "flash_functions.h"
#include "../Common/diag_functions.h"
#include "fw_update_req_handlers.h"
#include "tx_functions.h"

// TO DO - maybe add Node's Page ID to FW Missing Response

// FW Update variables
WORD fwUpgradeTimer = 0;  // 16 bits (0 = not in fw Upgrade)
BYTE fwPageBuffer[BLOCKS_PER_PAGE][BLOCK_SIZE_IN_BYTES];
BYTE fwVerMD5SumForPage[FW_MD5_SUM_SIZE];

// static variables only used by this file
static BYTE fwVerLoading[FW_VER_SIZE] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };  // Copy from FW UPDATE REQ
static WORD fwUpgradeSize = 0;
static BYTE fwVerMD5sumForFile[FW_MD5_SUM_SIZE];
static CHAR8 fwPageId = -1;  // -1 means didn't rx 1st Data header request with next page #
static BYTE fwBlockId = 0;
static WORD fwBlocksRxedThisPageBitMap = 0x00;  // 16 bits representing 16 code Blocks for current Page
static WORD goodFwPagesRxed = 0;
static bool fwFileRxed = false;

// Function prototypes
static WORD reqFwData_rxedPage(void);

/*****************************************************************************
  Function:
    void reqFwReset(void)

  Description:
    Resets the state of the firmware module to indicate that no firmware
    load is in progress.

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/

void reqFwReset(void)
{
    fwFileRxed = false;
    fwUpgradeSize = 0;
    fwUpgradeTimer = 0;

    // -1 means didn't rx 1st Data header request with next page #
    fwPageId = -1;
    fwBlockId = 0;
    fwBlocksRxedThisPageBitMap = 0x00;
}

/*****************************************************************************
  Function:
    void reqFwInfo_handler(void)

  Description:
    Process GW request for FW Info to this Node (P2P)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqFwInfo_handler(void)
{
    int ii = 0;

    txRespPrep(5);

    loadRespHeaderIntoTxBuff(RESP_FW_INFO, SIZE_RESP_FW_INFO);

    for (ii = 0; ii < FW_VER_SIZE;  ii++)
    {
        TX_buffer[TX_write_index][(ii+11)] = fwVerInUse[ii];
    }

    BYTE wiBit = 1 << (TX_write_index & 7);
    TX_low_power[TX_write_index / 8] &= ~wiBit;

    setTxBuffReady();
}


/*****************************************************************************
  Function:
    void reqFwMode_handler(void)

  Description:
    Process GW request for FW Mode to this Node (P2P)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqFwMode_handler(void)
{
    txRespPrep(5);

    loadRespHeaderIntoTxBuff(RESP_FW_MODE, SIZE_RESP_FW_MODE);

    // timer value of zero means not in f/w Upgrade
    TX_buffer[TX_write_index][11] = fwUpgradeTimer >> 8;
    TX_buffer[TX_write_index][12] = fwUpgradeTimer;

    BYTE wiBit = 1 << (TX_write_index & 7);
    TX_low_power[TX_write_index / 8] &= ~wiBit;

    setTxBuffReady();
}


/*****************************************************************************
  Function:
    void reqFwMissing_handler(void)

  Description:
    Process GW request for FW Missing to this Node (P2P)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqFwMissing_handler(void)
{
    CHAR8 rxPageId = (RX_packet[11] & 0x3F);
    CHAR8 txPageId = rxPageId;  // page Id from last HEADER request

    if (fwUpgradeTimer == 0)
    {
        txPageId |= 0xC0;  // bits 7&6 = 0x11 = 3 (not in FW Upgrade)
        countError(ERR_FW_MISSING_NOT_IN_FW_UPGRADE);
        // send response too
    }
    // else in f/w Upgrade
    else
    {
        fwUpgradeTimer = MAX_SECONDS_STAY_IN_FW_UPGRADE_MODE;  // restart timer

        // Check Rxed page ID unused bits are zero
        if ((RX_packet[11] & 0xC0) != 0x00)
            countError(ERR_FW_MISSING_PAGE_ID_BITS_7AND6);

        // Check Rxed page ID matches Page ID in last Fw Data Header req
        if (rxPageId < fwPageId)
        {
            txPageId |= 0x80;  // bits 7&6 = 0x10 = 2 (Page ID <)
            countError(ERR_FW_MISSING_PAGE_ID_LESS);
        }
        else if (rxPageId > fwPageId)
        {
            txPageId |= 0x40;  // bits 7&6 = 0x01 = 1 (Page ID >)
            countError(ERR_FW_MISSING_PAGE_ID_GREATER);
        }
    }

    txRespPrep(5);

    loadRespHeaderIntoTxBuff(RESP_FW_MISSING, SIZE_RESP_FW_MISSING);

    TX_buffer[TX_write_index][11] = txPageId;
    TX_buffer[TX_write_index][12] = fwBlocksRxedThisPageBitMap >> 8;
    TX_buffer[TX_write_index][13] = fwBlocksRxedThisPageBitMap;

    BYTE wiBit = 1 << (TX_write_index & 7);
    TX_low_power[TX_write_index / 8] &= ~wiBit;

    setTxBuffReady();
}


/*****************************************************************************
  Function:
    void reqFwUpgrade_handler(void)

  Description:
    Process GW request for FW Upgrade (Multicast)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqFwUpgrade_handler(void)
{
    int ii = 0;

    if (fwUpgradeTimer > 0)
        countError(ERR_FW_UPGRADE_ALREADY_IN_UPGRADE);  // bump err but restart Upgrade

    for (ii = 0; ii < FW_MD5_SUM_SIZE;  ii++)
    {
        fwVerMD5sumForFile[ii] = RX_packet[(ii+33)];
    }
    // NOTE: Can't validate file's MD5 Sum until have all pages

    for (ii = 0; ii < FW_VER_SIZE;  ii++)
    {
        fwVerLoading[ii] = RX_packet[(ii+11)];
    }

    fwUpgradeSize = ((WORD)RX_packet[31] << 8) + RX_packet[32];

    fwUpgradeTimer = MAX_SECONDS_STAY_IN_FW_UPGRADE_MODE;  //start timer non zero (0 = not in fw Upgrade)

    // Make sure flags reset until get 1st FW Data Header req
    fwBlocksRxedThisPageBitMap = 0x0000;  // 0 bits = missing
    fwPageId = -1;  // -1 means didn't rx 1st Data Header req yet
    fwBlockId = 0x00;
    goodFwPagesRxed = 0;
}



/*****************************************************************************
  Function:
    void reqFwDataHeader_handler(void)

  Description:
    Process GW request for FW Data Header (Multicast)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqFwDataHeader_handler(void)
{
    int ii = 0;
    BYTE rxPageId = 0;

    if (fwUpgradeTimer == 0)
    {
        countError(ERR_FW_HDR_NOT_IN_FW_UPGRADE);
        return;
        // Maybe powered up, or reset, in the middle of a f/w upgrade
    }
    else
        fwUpgradeTimer = MAX_SECONDS_STAY_IN_FW_UPGRADE_MODE;  // restart timer

    // Check if got header with same Page Id as last time
    rxPageId = (RX_packet[11] & 0x3F);
    if (rxPageId == fwPageId)
    {
        countError(ERR_FW_HDR_SAME_PAGE_ID);
        return;
        // Don't clear block bit map - don't do flash writes for data already rxed
    }

    // Check if GW changed Page ID before this node got all blocks last Page
    if ((fwBlocksRxedThisPageBitMap != 0xFFFF) && (rxPageId != 0))
    {
        countError(ERR_FW_HDR_WHEN_MISSING_BLKS);
        return;
    }

    // Make sure don't overflow flash if Page ID too large for this PIC
    else if (rxPageId > MAX_PAGE_ID_FOR_THIS_PIC)
    {
        countError(ERR_FW_HDR_RX_PAGE_ID_TOO_BIG_FOR_HW);
        return;
    }

    // Else a good FW Upgrade request
    for (ii = 0; ii < FW_MD5_SUM_SIZE;  ii++)
    {
        fwVerMD5SumForPage[ii] = RX_packet[(ii+12)];
    }
    // NOTE: can't validate this MD5 Sum until rx all 16 good data blocks for this page ID and put them into RAM

    fwBlocksRxedThisPageBitMap = 0;  // 0 bits = all 16 blocks in current page missing
    fwPageId = rxPageId;  // save 6-bit Page Id to match again FW DATA requests that should follow
}


/*****************************************************************************
  Function:
    WORD reqFwData_handler(void)

  Description:
    Process GW request for FW Data (Multicast)

  Parameters:
    None

  Returns:
    1 = don't repeat packet (e.g., packet erred)
    0 = okay to repeat packet, if Node is a repeater
  ***************************************************************************/
WORD reqFwData_handler(void)
{
    int ii = 0;

    // Check in f/w Update Mode
    if (fwUpgradeTimer == 0)
    {
        countError(ERR_FW_DATA_NOT_IN_FW_UPGRADE);
        // Maybe powered up, or reset, in the middle of a f/w upgrade
        return 0;  // okay to repeat
    }
    else
        fwUpgradeTimer = MAX_SECONDS_STAY_IN_FW_UPGRADE_MODE;  // restart timer

    // Page ID bit 6 should be set, else ignore invalid DATA request
    if ((RX_packet[11] & 0x40) != 0x40)
    {
        countError(ERR_FW_DATA_PAGE_ID_BIT6_ZERO);
        return 1;  // don't repeat
    }

    // Valid Block ID should only uses bits 3-0 (0x0F), make sure bits 7-4 clear
    if ((RX_packet[12] & 0xF0) != 0x00)
    {
        countError(ERR_FW_DATA_BLOCK_ID_BITS_7TO4);
        return 1;  // don't repeat
    }

    // PageID match PageID rxed in last FW_DATA_HEADER req
    if (fwPageId != (RX_packet[11] & 0x3F))
    {
        countError(ERR_FW_DATA_PAGE_ID_MISMATCH);
        return 0;  // okay to repeat
        // This could happen if this Node lost the Header req and GW didn't verify that via the Missing Req
    }

    fwBlockId = (RX_packet[12] & 0x0F);  // 0-15

    // Create bit mask for rxed block number
    int rxBlocksBitMask = (1 << fwBlockId);  // 0x00001 (0), 0x0002 (1), 0x0004 (2), ... 0x1000 (15)

    // Check if already received this block in this page
    if ((fwBlocksRxedThisPageBitMap & rxBlocksBitMask) != 0)
    {
        countError(FW_DATA_DUPLICATES);  // statistic
        return 0;  // okay to repeat
    }

    //Copy data to next Block's spot in RAM Page Buffer
    for (ii = 0; ii < BLOCK_SIZE_IN_BYTES;  ii++)
    {
        fwPageBuffer[fwBlockId][ii] = RX_packet[(ii+13)];
    }

    fwBlocksRxedThisPageBitMap |= rxBlocksBitMask;  // 1 bit in bit map = block rxed

    // Check if rxed all blocks of current page yet
    if (fwBlocksRxedThisPageBitMap != 0xFFFF)
    {
        // if not 1st block - check if rxed this data block in sequential order
        if (fwBlockId != 1)
        {
            rxBlocksBitMask = rxBlocksBitMask >> 1;
            if ((fwBlocksRxedThisPageBitMap & rxBlocksBitMask) == 0)
                countError(ERR_FW_DATA_OUT_OF_ORDER);
        }
        return 0;  // okay to repeat (not last block of page)
    }

    return(reqFwData_rxedPage());
}


/*****************************************************************************
  Function:
    static WORD reqFwData_rxedPage(void)

  Description:
    Fnc for processing last received block of current page

  Parameters:
    None

  Returns:
    1 = don't repeat packet (e.g., packet erred)
    0 = okay to repeat packet, if Node is a repeater
  ***************************************************************************/
static WORD reqFwData_rxedPage(void)
{
    // Rxed all 16 blocks of current page
    // Check if rxed too many pages for file
    if (((goodFwPagesRxed + 1) * BLOCK_SIZE_IN_BYTES) > fwUpgradeSize)
    {
        countError(ERR_FW_FILE_RXING_TOO_BIG);
        return 0;  // okay to repeat - maybe a s/w bug caused this err
        // Let FW Update Timer expire or GW can issue another FW_UPDATE command
    }

    // Check if Page's MD5 Sum Valid
    if (!fwPageValidPerMD5Check((uint8_t *)fwPageBuffer, PAGE_SIZE_IN_BYTES, (goodFwPagesRxed + 1), fwVerMD5SumForPage))
    {
         // MD5 Sum mismatch on Page - wait for GW to send all data blocks of this page again - clean-up:
        fwBlocksRxedThisPageBitMap = 0x0000;  // GW will get this bit map on next FW_MISSING request
        countError(ERR_PAGE_MD5_SUM_MISMATCH);
        return 0;  // okay to repeat - maybe the data error was in a previous block of this page
    }

    // Erase flash before can write to it
    flash_erase(fwPageId);
    // Write page to 2nd half of flash to free up RAM Page buffer for next page from GW
    flash_page(fwPageId, fwPageBuffer /*src addr in RAM*/);

    // Check if just rxed the last Page of file
    goodFwPagesRxed++;
    if ((goodFwPagesRxed * PAGE_SIZE_IN_BYTES) == fwUpgradeSize)
    {
        // Check MD5 Sum of file matches
        if (!fwFileValidPerMD5Check(fwVerMD5sumForFile))
        {
            // MD5 Sum Mismatch on File - wait for GW to send all data blocks of all pages again - clean-up:
            fwBlocksRxedThisPageBitMap = 0x0000;  // GW will rx this bit map on next FW_MISSING request
                                                  // plus GW will get indication that nodes Page < GW Page
            fwPageId = -1;  // -1 means didn't rx 1st Data Header req yet
            fwBlockId = 0x00;
            goodFwPagesRxed = 0;
            countError(ERR_FILE_MD5_SUM_MISMATCH);
            return 0;  // okay to repeat - maybe the data error was in a previous block of this page
        }

        // File Valid
        fwFileRxed = true;  // flag used by FW Apply req handler to copy file from 2nd to 1st half of flash
    }

    return 0;  // okay to repeat
}



/*****************************************************************************
  Function:
    void reqFwApply_handler(void)

  Description:
    Process GW request for FW Data Header (Multicast)

  Parameters:
    None

  Returns:
    None
  ***************************************************************************/
void reqFwApply_handler( WORD chn_rpt, DWORD gw )
{
    uint16_t page_count = 0;

    if (fwUpgradeTimer == 0)
    {
        countError(ERR_FW_APPLY_NOT_IN_FW_UPGRADE);
        return;
        // Maybe powered up, or reset, in the middle of a f/w upgrade
    }

    if (!fwFileRxed)
    {
        countError(ERR_FW_APPLY_FILE_NOT_ALL_RXED);
        return;
    }

    if (memcmp(fwVerLoading /* Ver in prev Update req*/, &RX_packet[11] /*Ver in this Apply req*/, FW_VER_SIZE) != 0)
    {
        countError(ERR_FW_APPLY_VER_MISMATCH);
        return;
    }

    page_count = fwPageId + 1;

    // Reset variables for next fw Upgrade incase for
    // some reason flash_boot_entry() doesn't reset s/w
    reqFwReset();

    /* Ensure the watchdog doesn't time out */
    ClrWdt();

    // Call fnc to copy New f/w in 2nd half of flash over 1st half flash's f/w and reset
    flash_boot_entry(page_count, chn_rpt, gw);
    // Note: should not return from above fnc call - should reset and start running new f/w
}
