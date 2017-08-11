/*********************************************************************
 * fw_update_req_handlers.h
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#ifndef FW_UPDATE_REQ_HANDLERS_H
#define	FW_UPDATE_REQ_HANDLERS_H

#include "GenericTypeDefs.h"

// Page size = 15356 bytes, 64 pages max (0-63)
// 1 Page = 16 blocks
// Block size = 96 bytes, 16 block max (0-15)
#define BLOCKS_PER_PAGE 16
#define BLOCK_SIZE_IN_BYTES 96
#define PAGE_SIZE_IN_BYTES (BLOCKS_PER_PAGE * BLOCK_SIZE_IN_BYTES)
#define PAGE_SIZE_IN_WORDS 768
#define MAX_PAGE_ID_FOR_THIS_PIC (20)  // per FW Data 6-bit PageId field, 0-64, but For PIC24FJ64GA line above, 0x5500 = 20480/1024 = 20 pages - 1024 = (1536+512)/2
#define MAX_SECONDS_STAY_IN_FW_UPGRADE_MODE 600

// Request/Response command codes:
#define REQ_FW_INFO         210
#define REQ_FW_UPGRADE      212
#define REQ_FW_MODE         214
#define REQ_FW_DATA_HEADER_OR_DATA  216
#define REQ_FW_MISSING      218
#define REQ_FW_APPLY        220

#define RESP_FW_INFO        211
#define RESP_FW_MISSING     219
#define RESP_FW_MODE        215

// Size of req/resp:
#define SIZE_REQ_FW_INFO    10
#define SIZE_REQ_FW_UPGRADE 48
#define SIZE_REQ_FW_MODE    10
#define SIZE_REQ_FW_DATA_HEADER 27
#define SIZE_REQ_FW_DATA    108
#define SIZE_REQ_FW_MISSING 11
#define SIZE_REQ_FW_APPLY   30

#define SIZE_RESP_FW_INFO   30
#define SIZE_RESP_FW_MODE   12
#define SIZE_RESP_FW_MISSING    13

void reqFwReset(void);
void reqFwInfo_handler(void);
void reqFwMode_handler(void);
void reqFwMissing_handler(void);
void reqFwUpgrade_handler(void);
void reqFwDataHeader_handler(void);
WORD reqFwData_handler(void);
void reqFwApply_handler( WORD chn_rpt, DWORD gw );
void countRadioErr(int radioErrType);

#endif	/* FW_UPDATE_REQ_HANDLERS_H */
