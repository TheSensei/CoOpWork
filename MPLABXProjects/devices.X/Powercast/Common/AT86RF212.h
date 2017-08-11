/*********************************************************************
 * AT86RF212.h
 * Written by Daniel W Harrist
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/
#include "HardwareProfile.h"
#include "../../Microchip/Include/GenericTypeDefs.h"

// Support for multiple radio regions

#define North_America 1
#define United_Kingdom 2

// #define RADIO_REGION North_America

#if RADIO_REGION == North_America
#define MAX_TX_POWER 11
#define MIN_TX_POWER -25
#define CONNECTOR_CHAN_1 CHAN_909
#define CONNECTOR_CHAN_2 CHAN_921
#define FEM_POWER MINUS_6
#define NOFEM_POWER PLUS_5
#define MODULATION 0x14 /* BPSK-40 */
#define CERT_BAND 6
#define CERT_CHAN_LOW B_6_CHAN_902_5
#define CERT_CHAN_MED B_6_CHAN_915
#define CERT_CHAN_HIGH B_6_CHAN_927_3

#elif RADIO_REGION == United_Kingdom
#define MAX_TX_POWER -11
#define MIN_TX_POWER -25
#define CONNECTOR_CHAN_1 CHAN_915_9
#define CONNECTOR_CHAN_2 CHAN_917_3
#define NOFEM_POWER PLUS_5
#define FEM_POWER MINUS_11
#define MODULATION 0 /* BPSK 20 */
#define CERT_BAND 3
#define CERT_CHAN_LOW CHAN_915_9
#define CERT_CHAN_MED CHAN_916_6
#define CERT_CHAN_HIGH CHAN_917_3

#else
#error "Unsupported Radio Region"
#endif

#if HAS_FEM == 1
#define CERT_POWER FEM_POWER
#else
#define CERT_POWER NOFEM_POWER
#endif


/************************ FUNCTION PROTOTYPES **********************/
void SPIPut(BYTE);
BYTE SPIGet(void);
void TRX_RegisterSet(BYTE , BYTE);
BYTE TRX_RegisterRead(BYTE);
void TRX_Reset(void);
void TRX_Init(void);
BYTE TRX_Calibrate(void);
WORD TRX_SetState(BYTE);
void TRX_ChangeChannel(BYTE);
BYTE TRX_dBmToPower(int);
void TRX_PowerLevel(BYTE);
void TRX_CheckRXInt(void);
void TRX_CheckIntStatus(void);
void TRX_WriteData(BYTE);
BYTE TRX_TxPacket(void);
void TRX_ClearFrame(void);
#ifndef TX_ONLY
int TRX_RXMessage(void);
BYTE TRX_GetLength(void);
BYTE TRX_GetData(int);
BYTE TRX_GetRSSI(void);
void TRX_ReleasePacket(void);
#endif
void TRX_DWE_TX(void);
void TRX_DWE_RX(void);
void TRX_InitAES(void);
void TRX_InitAES_RX(void);
void TRX_RunAES_TX(void);
void TRX_RunAES_RX(void);
BYTE TRX_CheckError(void);
unsigned char TRX_GetEightRandomBits(void);
void inline enable_radio_interrupts(int old_ipl);
int inline disable_radio_interrupts(void);

/************************ ERRORS ***********************************/
#define TRX_NO_ERROR                0
#define TRX_BUFFER_OVERUN           1
#define TRX_MISSED_IRQ              2
#define TRX_FAILED_CAL              3
#define TRX_FAILED_STATE_CHANGE     4
#define TRX_FAILED_TX               5
#define TRX_FAILED_TX_TIMEOUT       6
#define TRX_RX_CRC_ERR              7
#define TRX_DROP_NEWEST_APP_RX_BUF  8
#define TRX_DROP_OLDEST_APP_RX_BUF  9
#define TRX_RX_FRAME_TOO_LARGE      10

/************************ MACROS ***********************************/
// Frequencies for Band 3
#define CHAN_903        0
#define CHAN_904        10
#define CHAN_905        20
#define CHAN_906        30
#define CHAN_907        40
#define CHAN_908        50
#define CHAN_909        60
#define CHAN_910        70
#define CHAN_911        80
#define CHAN_912        90
#define CHAN_913        100
#define CHAN_914        110
#define CHAN_915        120
#define CHAN_915_9      129
#define CHAN_916        130
#define CHAN_916_6      136
#define CHAN_917        140
#define CHAN_917_3      143
#define CHAN_918        150
#define CHAN_919        160
#define CHAN_920        170
#define CHAN_921        180
#define CHAN_922        190
#define CHAN_923        200
#define CHAN_924        210
#define CHAN_925        220
#define CHAN_926        230
#define CHAN_927        240
#define CHAN_928        250

/* Frequencies for Atmel Band 6 */
#define B_6_CHAN_902_5    7
#define B_6_CHAN_915    130
#define B_6_CHAN_927_3  253

// TX Power Levels - 212B version only
#define PLUS_11         0xC0    // +11
#define PLUS_10         0xC1    // +10
#define PLUS_9          0x80    // +9
#define PLUS_8          0x82    // +8
#define PLUS_7          0x83    // +7
#define PLUS_6          0x84    // +6
#define PLUS_5          0x40    // +5
#define PLUS_4          0x86    // +4
#define PLUS_3          0x00    // +3
#define PLUS_2          0x01    // +2
#define PLUS_1          0x02    // +1
#define ZERO_DBM        0x03    //  0 dBm
#define MINUS_1         0x04    // -1
#define MINUS_2         0x27    // -2
#define MINUS_3         0x05    // -3
#define MINUS_4         0x07    // -4
#define MINUS_5         0x08    // -5
#define MINUS_6         0x91    // -6
#define MINUS_7         0x09    // -7
#define MINUS_8         0x0B    // -8
#define MINUS_9         0x0C    // -9
#define MINUS_10        0x0D    // -10
#define MINUS_11        0x0E    // -11
#define MINUS_12        0x0F    // -12
#define MINUS_13        0x10    // -13
#define MINUS_14        0x11    // -14
#define MINUS_15        0x12    // -15
#define MINUS_16        0x13    // -16
#define MINUS_17        0x14    // -17
#define MINUS_18        0x15    // -18
#define MINUS_19        0x16    // -19`
#define MINUS_20        0x17    // -20
#define MINUS_21        0x19    // -21
#define MINUS_22        0x1A    // -22
#define MINUS_23        0x1B    // -23
#define MINUS_24        0x1C    // -24
#define MINUS_25        0x1D    // -25

// Frame settings
#define RX_LENGTH_SIZE  1    // 1 byte at start of rx buffer
#define RX_RSSI_SIZE    1    // 1 byte at end of rx buffer
#define RX_BUFFER       0x0800 // must be a power of 2, must be < 0xffff - FRAME_PAYLOAD_MAX_LENGTH + RX_LENGTH_SIZE + RX_RSSI_SIZE
#define RX_BUFFER_SIZE_MASK (RX_BUFFER - 1)
#define RX_BUFFER_OFFSET(offset) ((offset) & RX_BUFFER_SIZE_MASK)
#define RADIO_TIMEOUT   5000    // x 10us
#define RSSI_BASE_VAL   100
#define PSDU_PAYLOAD_MAX_LENGTH 127
#define PSDU_CRC_LENGTH 2
#define FRAME_PAYLOAD_MAX_LENGTH (PSDU_PAYLOAD_MAX_LENGTH - PSDU_CRC_LENGTH)
#define FRAME_PAYLOAD_LENGTH(phr) ((phr) - PSDU_CRC_LENGTH)

// SPI Masking
#define SPI_TO          1000
#define WRITE_REG       0xC0
#define READ_REG        0x80
#define WRITE_FRAME     0x60
#define READ_FRAME      0x20
#define WRITE_SRAM      0x40
#define READ_SRAM       0x00
#define AES_ADDRESS     0x83
#define DW_KEY1         0x6D
#define DW_KEY2         0x32
#define AES_KEY_1       0xF2
#define AES_KEY_2       0xDD
#define AES_KEY_3       0x63
#define AES_KEY_4       0x5E
#define AES_KEY_5       0x01
#define AES_KEY_6       0xF7
#define AES_KEY_7       0x89
#define AES_KEY_8       0x73
#define AES_KEY_9       0xA5
#define AES_KEY_10      0x54
#define AES_KEY_11      0x27
#define AES_KEY_12      0x00
#define AES_KEY_13      0x0F
#define AES_KEY_14      0xE3
#define AES_KEY_15      0x44
#define AES_KEY_16      0xDA

// Address mapping for AT86RF212 Registers
#define TRX_STATUS      0x01
#define TRX_STATE       0x02
#define TRX_CTRL_0      0x03
#define TRX_CTRL_1      0x04
#define PHY_TX_PWR      0x05
#define PHY_RSSI        0x06
#define   PHY_RSSI_RX_CRC_VALID 0x80
#define PHY_ED_LEVEL    0x07
#define PHY_CC_CCA      0x08
#define CCA_THRES       0x09
#define RX_CTRL         0x0A    //not defined at this address
#define SFD_VALUE       0x0B
#define TRX_CTRL_2      0x0C
#define ANT_DIV         0x0D
#define IRQ_MASK        0x0E
#define IRQ_STATUS      0x0F
#define VREG_CTRL       0x10    // used for now
#define BATMON          0x11    // used for now
#define XOSC_CTRL       0x12
#define CC_CTRL_0       0x13
#define CC_CTRL_1       0x14
#define RX_SYN          0x15
#define RF_CTRL_0       0x16
#define XAH_CTRL_1      0x17
#define FTN_CTRL        0x18
#define RF_CTRL_1       0x19
#define PLL_CF          0x1A
#define PLL_DCU         0x1B
#define PART_NUM        0x1C
#define VERSION_NUM     0x1D
#define MAN_ID_0        0x1E
#define MAN_ID_1        0x1F
#define SHORT_ADDR_0    0x20
#define SHORT_ADDR_1    0x21
#define PAN_ID_0        0x22
#define PAN_ID_1        0x23
#define IEEE_ADDR_0     0x24
#define IEEE_ADDR_1     0x25
#define IEEE_ADDR_2     0x26
#define IEEE_ADDR_3     0x27
#define IEEE_ADDR_4     0x28
#define IEEE_ADDR_5     0x29
#define IEEE_ADDR_6     0x2A
#define IEEE_ADDR_7     0x2B
#define XAH_CTRL_0      0x2C
#define CSMA_SEED_0     0x2D
#define CSMA_SEED_1     0x2E
#define CSMA_BE         0x2F
#define TST_CTRL_DIG    0x36

// TRX Statuses & States
#define P_ON            0x00
#define BUSY_RX         0x01
#define BUSY_TX         0x02
#define TX_START        0x02
#define FORCE_TRX_OFF   0x03
#define FORCE_PLL_ON    0x04
#define RX_ON           0x06
#define TRX_OFF         0x08
#define PLL_ON          0x09
#define SLEEP           0x0F    // unreadable
#define BUSY_RX_AACK    0x11
#define BUSY_TX_ARET    0x12
#define RX_AACK_ON      0x16
#define TX_ARET_ON      0x19
#define RX_ON_NOCLK     0x1C
#define RX_AACK_NOCLK   0x1D
#define BUSY_AACK_NOCLK 0x1E
#define STATE_TRANS     0x1F

// IRQs
#define PLL_LOCK        0x01
#define PLL_UNLOCK      0x02
#define RX_START        0x04
#define TRX_END         0x08
#define CCA_ED_DONE     0x10
#define AMI             0x20
#define TRX_UR          0x40
#define BAT_LOW         0x80
