/********************************************************************
* FileName:		Console.c
* Dependencies: Console.h
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16, PIC18 Explorer
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).  
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*   This file configures and provides the function for using the
*   UART to transmit data over RS232 to the computer.
*
* Change History:
*  Rev   Date         Description
*  0.1   11/09/2006   Initial revision
*  1.0   01/09/2007   Initial release
*  2.0   4/24/2009    Modified for MiApp interface
*  2.1   6/20/2009    Add LCD support
*  3.1   5/28/2010    MiWi DE 3.1
********************************************************************/

/************************ HEADERS **********************************/
#include "WirelessProtocols\Console.h"
#include "SystemProfile.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"

#if defined(ENABLE_CONSOLE)

/************************ VARIABLES ********************************/
ROM unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/************************ FUNCTIONS ********************************/

        
/*********************************************************************
* Function:         void ConsoleEnable(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    enable UART1
*
* Note:			    None
********************************************************************/
void ConsoleEnable(void)
{
	U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    // Enable RX int
    IFS0bits.U1RXIF = 0;
	IEC0bits.U1RXIE = 1;
}


/*********************************************************************
* Function:         void ConsoleDisable(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    disable UART1
*
* Note:			    None
********************************************************************/
void ConsoleDisable(void)
{
	// Disable UART1
    U1MODEbits.UARTEN = 0;
    U1STAbits.UTXEN = 0;

    // Disable RX interrupt
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 0;

}


/*********************************************************************
* Function:         void ConsolePutROMString(ROM char* str)
*
* PreCondition:     none
*
* Input:		    str - ROM string that needs to be printed
*
* Output:		    none
*
* Side Effects:	    str is printed to the console
*
* Overview:		    This function will print the inputed ROM string
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void ConsolePutROMString(ROM char* str)
{
    BYTE c;

    while( (c = *str++) )
        ConsolePut(c);
}

/*********************************************************************
* Function:         void ConsolePut(BYTE c)
*
* PreCondition:     none
*
* Input:		    c - character to be printed
*
* Output:		    none
*
* Side Effects:	    c is printed to the console
*
* Overview:		    This function will print the inputed character
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void ConsolePut(BYTE c)
{
    while(U1STAbits.TRMT == 0);
    U1TXREG = c;
}

/*********************************************************************
* Function:         BYTE ConsoleGet(void)
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    one byte received by UART
*
* Side Effects:	    none
*
* Overview:		    This function will receive one byte from UART
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
BYTE ConsoleGet(void)
{
	char Temp;

    while(IFS0bits.U1RXIF == 0);

	Temp = U1RXREG;
    IFS0bits.U1RXIF = 0;
	return Temp;
}
	


/*********************************************************************
* Function:         void PrintChar(BYTE toPrint)
*
* PreCondition:     none
*
* Input:		    toPrint - character to be printed
*
* Output:		    none
*
* Side Effects:	    toPrint is printed to the console
*
* Overview:		    This function will print the inputed BYTE to 
*                   the console in hexidecimal form
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void PrintChar(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
* Function:         void PrintDec(BYTE toPrint)
*
* PreCondition:     none
*
* Input:		    toPrint - character to be printed. Range is 0-99
*
* Output:		    none
*
* Side Effects:	    toPrint is printed to the console in decimal
*                   
*
* Overview:		    This function will print the inputed BYTE to 
*                   the console in decimal form
*
* Note:			    Do not power down the microcontroller until 
*                   the transmission is complete or the last 
*                   transmission of the string can be corrupted.  
********************************************************************/
void PrintDec(BYTE toPrint)
{
    ConsolePut(CharacterArray[toPrint/10]);
    ConsolePut(CharacterArray[toPrint%10]);
}

#endif  //ENABLE_CONSOLE
