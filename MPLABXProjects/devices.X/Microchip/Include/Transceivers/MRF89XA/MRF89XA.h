/********************************************************************
* FileName:		MRF89XA.h
* Dependencies: none
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.30 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
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
*  Defines configurations and the address, channel settings for the 
*   MRF89XA
*
* Change History:
*  Rev   Date         Author    Description
*  3.1   5/28/2010    SM        MiWi DE 3.1
********************************************************************/

#if !defined(__MRF89XA_H) && defined(MRF89XA)
    #define __MRF89XA_H
    
//	#define	APPLICATION_CIRCUIT_950				//enable this if Application circuit is built for supporting 950-960 MHz instead of 860-870
												//but the band selection must still indicate 863 (BAND_863)

    #include "SystemProfile.h"
    #include "GenericTypeDefs.h"
    #include "Compiler.h"
    #include "WirelessProtocols/SymbolTime.h"
    #include "Transceivers/MRF89XA/ConfigMRF89XA.h"
    #include "Transceivers/Security.h"
    #include "Transceivers/MCHP_MAC.h"
    #include "TimeDelay.h"
    
    /********************************************************************
    MRF89XA Operating modes
    **********************************************************************/
    #define RF_SLEEP        0x00
    #define RF_STANDBY      0x20
    #define RF_SYNTHESIZER  0x40
    #define RF_RECEIVER     0x60
    #define RF_TRANSMITTER  0x80

    #define CHIPMODE_SLEEPMODE 0x00	//000 [7:5]
    #define CHIPMODE_STBYMODE  0x20	//001 ;default
    #define CHIPMODE_FSMODE    0x40	//010
    #define CHIPMODE_RX        0x60	//011
    #define CHIPMODE_TX        0x80	//100
    
    #define BITRATE_200     0x00
    #define BITRATE_100     0x01
    #define BITRATE_66      0x02
    #define BITRATE_50      0x03
    #define BITRATE_40      0x04
    #define BITRATE_25      0x07
    #define BITRATE_20      0x09
    #define BITRATE_10      0x13
    #define BITRATE_5       0x27
    #define BITRATE_2       0x63
    
    #define FREQBAND_902    0x00   //902-915 00 [4:3]
    #define FREQBAND_915    0x08   //915-928 01 ;default
    #define FREQBAND_863    0x10	//950-960 or 863-870 10
    
    #define BW_25  0x00
    #define BW_50  0x01
    #define BW_75  0x02
    #define BW_100 0x03
    #define BW_125 0x04
    #define BW_150 0x05
    #define BW_175 0x06
    #define BW_200 0x07
    #define BW_225 0x08
    #define BW_250 0x09
    #define BW_275 0x0A
    #define BW_300 0x0B
    #define BW_325 0x0C
    #define BW_350 0x0D
    #define BW_375 0x0E
    #define BW_400 0x0F
    
    #define FREQ_DEV_33  0x0B
    #define FREQ_DEV_40  0x09
    #define FREQ_DEV_50  0x07
    #define FREQ_DEV_67  0x05
    #define FREQ_DEV_80  0x04
    #define FREQ_DEV_100 0x03
    #define FREQ_DEV_133 0x02
    #define FREQ_DEV_200 0x01
    
    #define VCO_TRIM_00 0x00            // [2:1] Vtune determined by tank inductor values
    #define VCO_TRIM_01 0x02
    #define VCO_TRIM_10 0x04
    #define VCO_TRIM_11 0x06
 
    
    #define LNA_GAIN_0_DB   0x00        //00 [1:0] 0dB ;default (This is IF Filter gain)
    #define LNA_GAIN_45_DB  0x01        //01 -4.5dB
    #define LNA_GAIN_9_DB   0x02        //10 -9dB
    #define LNA_GAIN_135_DB 0x03        //11 -13.5dB

    #define TX_POWER_13_DB      0x00    //[3:1], 13dBm
    #define TX_POWER_10_DB      0x01	//10dBm
    #define TX_POWER_7_DB       0x02	//7dBm
    #define TX_POWER_4_DB       0x03	//4dBm
    #define TX_POWER_1_DB       0x04	//1dBm
    #define TX_POWER_N_2_DB     0x05	//-2dBm
    #define TX_POWER_N_5_DB     0x06	//-5dBm
    #define TX_POWER_N_8_DB     0x07	//-8dBm

    #define FILCON_SET_65   0x00		//65 KHz
    #define FILCON_SET_82   0x10		//82 KHz
    #define FILCON_SET_109  0x20
    #define FILCON_SET_137  0x30
    #define FILCON_SET_157  0x40
    #define FILCON_SET_184  0x50
    #define FILCON_SET_211  0x60
    #define FILCON_SET_234  0x70
    #define FILCON_SET_262  0x80
    #define FILCON_SET_321  0x90
    #define FILCON_SET_378  0xA0
    #define FILCON_SET_414  0xB0
    #define FILCON_SET_458  0xC0
    #define FILCON_SET_514  0xD0
    #define FILCON_SET_676  0xE0
    #define FILCON_SET_987  0xF0
   
    //Define the R, P, S sets for start channels
//    #define	START_FREQUENCY 902000
    #define	RVALUE  100
    #define	R1CNT   119		//center freq - 903MHz
    #define	P1CNT   99
    #define	S1CNT   25                   	
    #define	R2CNT   100
    #define	P2CNT   85
    #define	S2CNT   55
//    #define	CHANNEL_SPACING	 400			//Channel spacing 400KHz
//    #define CHANNEL_NUM 64
    #define FULL_CHANNEL_MAP 0xFFFFFFFF
    
    #define CHAN_1	1
	#define CHAN_2	2

    #define	FXTAL					12.8					//Frequency of the crystal in MHz
    
    #if defined(DATA_RATE_2)
        #define         DATARATE                 BITRATE_2
        #define			BANDWIDTH				 BW_50                
        #define 		FREQ_DEV				 FREQ_DEV_33
        #define			FILCON_SET				 FILCON_SET_157	 
    #elif defined(DATA_RATE_5)
        #define         DATARATE                 BITRATE_5
        #define			BANDWIDTH				 BW_50                
        #define 		FREQ_DEV				 FREQ_DEV_33
        #define			FILCON_SET				 FILCON_SET_157				           
    #elif defined(DATA_RATE_10)
        #define         DATARATE                 BITRATE_10
        #define			BANDWIDTH				 BW_50                
        #define 		FREQ_DEV				 FREQ_DEV_33
        #define			FILCON_SET				 FILCON_SET_157				                      
    #elif defined(DATA_RATE_20)
        #define         DATARATE                 BITRATE_20
        #define			BANDWIDTH				 BW_75                
        #define 		FREQ_DEV				 FREQ_DEV_40
        #define			FILCON_SET				 FILCON_SET_234				           
    #elif defined(DATA_RATE_40)
        #define         DATARATE                 BITRATE_40
        #define			BANDWIDTH				 BW_150                
        #define 		FREQ_DEV				 FREQ_DEV_80
        #define			FILCON_SET				 FILCON_SET_414
    #elif defined(DATA_RATE_50)
        #define         DATARATE                 BITRATE_50
        #define			BANDWIDTH				 BW_175                
        #define 		FREQ_DEV				 FREQ_DEV_100
        #define			FILCON_SET				 FILCON_SET_514
    #elif defined(DATA_RATE_66)
        #define         DATARATE                 BITRATE_66
        #define			BANDWIDTH				 BW_250                
        #define 		FREQ_DEV				 FREQ_DEV_133
        #define			FILCON_SET				 FILCON_SET_676
    #elif defined(DATA_RATE_100)
        #define         DATARATE                 BITRATE_100
        #define			BANDWIDTH				 BW_400                
        #define 		FREQ_DEV				 FREQ_DEV_200
        #define			FILCON_SET				 FILCON_SET_987
    #elif defined(DATA_RATE_200)
        #define         DATARATE                 BITRATE_200
        #define			BANDWIDTH				 BW_400
        #define 		FREQ_DEV				 FREQ_DEV_200
        #define			FILCON_SET				 FILCON_SET_987
    #else
        #error          "No valid data rate defined"     
    #endif
		
    	//default register settings
    #define GCONREG_SET     (VCO_TRIM_11)
    #define DMODREG_SET     (0x84 | LNA_GAIN)
    #define FLTHREG_SET     (0x0C)
    #define FIFOCREG_SET    (0xC1)								//FIFO size = 64 bytes and threshold limit for IRQ is 1
    #define PACONREG_SET    (0x38)
    #define FTXRXIREG_SET   (0xC8)
    #define FTPRIREG_SET    (0x0D)
    #define RSTHIREG_SET    (0x00)	
    #define FILCONREG_SET   (FILCON_SET | BANDWIDTH)
    #define PFILCREG_SET    (0x38)
    #define SYNCREG_SET     (0x38)
    #define RESVREG_SET     (0x07)
    #define SYNCV31REG_SET  (0x69)
    #define SYNCV23REG_SET  (0x81)
    #define SYNCV15REG_SET  (0x7E)
    #define SYNCV07REG_SET  (0x96)
    #define TXPARAMREG_SET  (0xF0 | (TX_POWER<<1))
    #define CLKOUTREG_SET   (0x88)
    #define PLOADREG_SET    (0x40)
    #define NADDREG_SET     (0x00)
    #define PCONREG_SET     (0xE8)
    #define FCRCREG_SET     (0x00)
															
		//register description
    #define GCONREG     0x0000
    #define DMODREG     0x0200
    #define FDEVREG     0x0400
    #define BRREG       0x0600
    #define FLTHREG     0x0800
    #define FIFOCREG    0x0A00	
    #define R1CNTREG    0x0C00
    #define P1CNTREG    0x0E00
    #define S1CNTREG    0x1000
    #define R2CNTREG    0x1200
    #define P2CNTREG    0x1400
    #define S2CNTREG    0x1600
    #define PACONREG    0x1800
    #define FTXRXIREG   0x1A00
    #define FTPRIREG    0x1C00
    #define RSTHIREG    0x1E00
    #define FILCONREG   0x2000
    #define PFILCREG    0x2200
    #define SYNCREG     0x2400
    #define RESVREG     0x2600
    #define RSTSREG     0x2800
    #define OOKCREG     0x2A00
    #define SYNCV31REG  0x2C00
    #define SYNCV23REG  0x2E00
    #define SYNCV15REG  0x3000
    #define SYNCV07REG  0x3200
    #define TXPARAMREG  0x3400
    #define CLKOUTREG   0x3600
    #define PLOADREG    0x3800
    #define NADDREG     0x3A00
    #define PCONREG     0x3C00
    #define FCRCREG     0x3E00
    
    #if defined(PROTOCOL_MIWI)
        #define PROTOCOL_HEADER_SIZE 11
    #endif
    
    #if defined(PROTOCOL_MIWI_PRO)
        #define PROTOCOL_HEADER_SIZE 11
    #endif
    
    #if defined(PROTOCOL_P2P)
        #define PROTOCOL_HEADER_SIZE 0
    #endif

    #if defined(ENABLE_SECURITY)
        #define TX_PACKET_SIZE (TX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+BLOCK_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+9)
        #define RX_PACKET_SIZE (RX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+BLOCK_SIZE+9)
    #else
        #define TX_PACKET_SIZE  (TX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+4)
        #define RX_PACKET_SIZE  (RX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+4)
    #endif


    #if RX_PACKET_SIZE > 64
        #warning  "Maximum RX PACKET SIZE should not be greater 64, redefine RX BUFFER SIZE"
        #undef RX_PACKET_SIZE
        #define RX_PACKET_SIZE 64
    #endif
    
    #if TX_PACKET_SIZE > 64
        #warning  "Maximum TX PACKET SIZE should not be greater 64, redefine TX BUFFER SIZE"
        #undef TX_PACKET_SIZE
        #define TX_PACKET_SIZE 64
    #endif


    typedef struct
    {
        union
        {
            BYTE    Val;
            struct
            {
                BYTE    Valid       :1;
            } bits;
        } flags; 
        BYTE        Payload[RX_PACKET_SIZE];
        BYTE        PayloadLen;
    } RX_PACKET;
    
    typedef struct
    {
        BOOL        Valid;
        BYTE        Seq;
        MIWI_TICK   startTick;
    } ACK_INFO;
    
    
#endif
