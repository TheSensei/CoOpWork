/*********************************************************************
 * md5_functions.h
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#ifndef MD5_FUNCTIONS_H
#define	MD5_FUNCTIONS_H

#define FW_MD5_SUM_SIZE 16

bool fwPageValidPerMD5Check(uint8_t * pPageAddress, uint16_t pageSize, WORD pageNumber, BYTE * pGwMD5SumForPage);
bool fwFileValidPerMD5Check(BYTE * pGwMD5SumForFile);

#endif	/* MD5_FUNCTIONS_H */

