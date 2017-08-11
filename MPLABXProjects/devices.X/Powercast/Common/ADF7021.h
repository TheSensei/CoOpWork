#include "HardwareProfile.h"

/************************ FUNCTION PROTOTYPES **********************/
void RF1_RegisterSet(BYTE, unsigned int, unsigned int);
void RF1_Init(int);
unsigned int RF1_GetRSSI(void);
void RF1_TXbyte(BYTE);
void RF1_SetState(BYTE);
void RF1_ChangeChannel(BYTE);
void RF1_SetPanID(BYTE);
void RF1_WriteData(BYTE);
void RF1_TxPacket(void);
void RF1_ClearFrame(void);
int RF1_RXMessage(void);
BYTE RF1_GetData(int);
void RF1_ReleasePacket(void);

/************************ MACROS ***********************************/
#define CHAN_1			0		// 903Mhz
#define CHAN_2			120		// 915Mhz
#define CHAN_3			240		// 927Mhz

#define TX_FRAME 	30
#define RX_FRAME 	TX_FRAME + 2	// includes LQI, ED, ignore RX_STATUS
#define MAX_BUFFER	5

#define RSSI_BASE_VAL	97

// Register Map
#define N_REG			0x00
#define VCO_REG			0x01
#define TXMOD_REG		0x02
#define TXRXCLK_REG		0x03
#define DEMOD_REG		0x04
#define IF_SET_REG		0x05
#define IF_CAL_REG		0x06
#define RDBK_REG		0x07
#define PWR_DN_REG		0x08
#define AGC_REG			0x09
#define AFC_REG			0x0A
#define SYNC_REG		0x0B
#define SWD_THR_REG		0x0C
#define FSK_DEMOD_REG	0x0D
#define TEST_DAC_REG	0x0E
#define TEST_REG		0x0F

// RF States
#define BUSY_RX			0x01
#define BUSY_TX			0x02
#define TX_START		0x02
#define FORCE_TRX_OFF	0x03
#define FORCE_PLL_ON	0x04
#define RX_ON			0x06
#define TRX_OFF			0x08
#define PLL_ON			0x09
