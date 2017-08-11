#include "HardwareProfile.h"

/************************ FUNCTION PROTOTYPES **********************/
void SPIPut(BYTE);
BYTE SPIGet(void);
void RF_SendCommand(int, BYTE);
void RF_Init(int);
void RF_WaitCTS(int);

void RF_RegisterSet(BYTE , BYTE);
BYTE RF_RegisterRead(BYTE);
void RF_SetState(BYTE);
void RF_ChangeChannel(BYTE);
void RF_SetPanID(BYTE);
void RF_WriteData(BYTE);
void RF_TxPacket(void);
void RF_ClearFrame(void);
int RF_RXMessage(void);
BYTE RF_GetData(int);
void RF_ReleasePacket(void);

/************************ MACROS ***********************************/
#define CHAN_1			0		// 903Mhz
#define CHAN_2			120		// 915Mhz
#define CHAN_3			240		// 927Mhz

#define TX_FRAME 	30
#define RX_FRAME 	TX_FRAME + 2	// includes LQI, ED, ignore RX_STATUS
#define MAX_BUFFER	5

#define RSSI_BASE_VAL	97

// SPI Masking
#define WRITE_REG		0xC0
#define READ_REG		0x80
#define WRITE_FRAME		0x60
#define READ_FRAME		0x20
#define SPI_TO			1000

// Commands
#define NOP						0x00
#define PART_INFO				0x01
#define POWER_UP				0x02
#define PATCH_IMAGE				0x04
#define FUNC_INFO				0x10
#define SET_PROPERTY			0x11
#define GET_PROPERTY			0x12
#define GPIO_PIN_CFG			0x13
#define GET_ADC_READING			0x14
#define FIFO_INFO				0x15
#define PACKET_INFO				0x16
#define IRCAL					0x17
#define PROTOCOL_CFG			0x18
#define GET_INT_STATUS			0x20
#define GET_PH_STATUS			0x21
#define GET_MODEM_STATUS		0x22
#define GET_CHIP_STATUS			0x23
#define START_TX				0x31
#define START_RX				0x32
#define REQUEST_DEVICE_STATE	0x33
#define CHANGE_STATE			0x34
#define RX_HOP					0x36
#define READ_CMD_BUFF			0x44
#define FRR_A_READ				0x50
#define FRR_B_READ				0x51
#define FRR_C_READ				0x53
#define FRR_D_READ				0x57
#define WRITE_TX_FIFO			0x66
#define READ_RX_FIFO			0x77

// RF States
#define BUSY_RX			0x01
#define BUSY_TX			0x02
#define TX_START		0x02
#define FORCE_TRX_OFF	0x03
#define FORCE_PLL_ON	0x04
#define RX_ON			0x06
#define TRX_OFF			0x08
#define PLL_ON			0x09

// IRQs
#define PLL_LOCK		0x01
#define PLL_UNLOCK		0x02
#define RX_START		0x04
#define TRX_END			0x08
#define CCA_ED_DONE		0x10
#define AMI				0x20
#define TRX_UR			0x40
#define BAT_LOW			0x80
