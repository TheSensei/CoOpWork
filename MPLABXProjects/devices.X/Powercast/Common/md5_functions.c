/*********************************************************************
 * md5_functions.c
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#include "xc.h"
#include "md5.h"
#include "stdbool.h"
#include "GenericTypeDefs.h"
#include "string.h"
#include "md5_functions.h"

static MD5_CONTEXT fileContext;
static uint8_t fileDigest[FW_MD5_SUM_SIZE];

// fnc prototypes
static void fwFileMD5AddNextPage(uint8_t * pPageAddress, uint16_t pageSize, WORD pageNumber);


/*****************************************************************************
  Function:
    bool fwPageValidPerMD5Check(uint8_t * pPageAddress, uint16_t pageSize, WORD pageNumber, BYTE * pGwMD5SumForPage)

  Description:
    Check if Fw Update Page data's calculated MD5 Sum matches page's MD5 Sum passed from GW

  Parameters:
   pPageAddress - address of page in RAM
   pageSize - size of page in bytes
   pageNumber - 0-15
   pGwMD5SumForPage - address of 16 byte MD5 Sum for Page provided by GW

  Returns:
    true - Page Valid
    false - Page NOT valid
  ***************************************************************************/
bool fwPageValidPerMD5Check(uint8_t * pPageAddress, uint16_t pageSize, WORD pageNumber, BYTE * pGwMD5SumForPage)
{
    MD5_CONTEXT pageContext;
    uint8_t pageDigest[FW_MD5_SUM_SIZE];

    MD5_Initialize(&pageContext);
    MD5_DataAdd(&pageContext, pPageAddress, pageSize );
    MD5_Calculate(&pageContext, pageDigest);

    if (memcmp(pGwMD5SumForPage, pageDigest, FW_MD5_SUM_SIZE) == 0)
    {
        // if Page MD5 Sum good - add page's data to File's MD5 Sum too
        fwFileMD5AddNextPage(pPageAddress, pageSize, pageNumber);
        return(true);  // page valid
    }
    else return (false); // page not valid
}


/*****************************************************************************
  Function:
    static void fwFileMD5AddNextPage(uint8_t * pPageAddress, uint16_t pageSize, WORD pageNumber)

  Description:
    Adds next Pages data to on-going MD5 Sum for File, to be checked after get last page of file

  Parameters:
   pPageAddress - address of page in RAM
   pageSize - size of page in bytes
   pageNumber - 0-15

  Returns:
    None
  ***************************************************************************/
static void fwFileMD5AddNextPage(uint8_t * pPageAddress, uint16_t pageSize, WORD pageNumber)
{
    // If 1st page of next file - initialize MD5 context
    if (pageNumber == 1)
        MD5_Initialize(&fileContext);

    MD5_DataAdd(&fileContext, pPageAddress, pageSize);
}


/*****************************************************************************
  Function:
    bool fwFileValidPerMD5Check(uint16_t fileSize, BYTE * pGwMD5SumForFile)

  Description:
    Check if Fw Update file calculated MD5 Sum matches file's MD5 Sum passed from GW

  Parameters:
   pGwMD5SumForFile - address of 16 byte MD5 Sum for File provided by GW

  Returns:
    true - File Valid
    false - File NOT valid
  ***************************************************************************/
bool fwFileValidPerMD5Check(BYTE * pGwMD5SumForFile)
{
    // Note: MD5_DataAdd() fcn called every time fwPageValidPerMD5Check() is called above)
    MD5_Calculate(&fileContext, fileDigest);

    return (memcmp(pGwMD5SumForFile, fileDigest, FW_MD5_SUM_SIZE) == 0);
}
