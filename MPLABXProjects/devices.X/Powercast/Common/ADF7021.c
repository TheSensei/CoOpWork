#include "ADF7021.h"

/************************ VARIABLES *********************************/
BYTE TX_frame[TX_FRAME];
BYTE RX_buffer[MAX_BUFFER][RX_FRAME];		// fifo buffer
BYTE TRX_state = 0;
int TX_packet_length;
int RX_int;
int RX_read_index;
int RX_write_index;
DWORD PanID[26] = {0xA7A7A730,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
				0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
				0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,};

// Set a register on the TRX
void RF1_RegisterSet(BYTE address, unsigned int val1, unsigned int val2)
{
	int outbit;
	BYTE RF1_IE_state = RF1_IE;
	
	RF1_IE = 0;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x8000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x4000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x2000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x1000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0800;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0400;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0200;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0100;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0080;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0040;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0020;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0010;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0008;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0004;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0002;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val1 & 0x0001;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x8000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x4000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x2000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x1000;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0800;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0400;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0200;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0100;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0080;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0040;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0020;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = val2 & 0x0010;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = address & 0x0008;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = address & 0x0004;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = address & 0x0002;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	outbit = address & 0x0001;

	if (outbit)
		RF1_SDATA = 1;
	else
		RF1_SDATA = 0;
		
	RF1_SCLK = 1;
	RF1_SCLK = 0;
	
	RF1_SLE = 1;
	RF1_SLE = 0;
	
	RF1_SDATA = 0;

	RF1_IE = RF1_IE_state;
}

unsigned int RF1_GetRSSI(void)
{
	unsigned int input = 0;
	
	RF1_SCLK = 0;
	RF1_SCLK = 1;
	
	if (RF1_SREAD == 1)
		input |= 0x0001;
		
		
	return input;
}

// Send 1 byte of data thru TRX
void RF1_TXbyte(BYTE value)
{
	int outbit = 0;
	
	while (RF1_DCLK == 1);

	outbit = value & 0x80;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);

	outbit = value & 0x40;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);
	
	outbit = value & 0x20;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);
	
	outbit = value & 0x10;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);
	
	outbit = value & 0x08;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);
	
	outbit = value & 0x04;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);
	
	outbit = value & 0x02;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
	while (RF1_DCLK == 1);
	
	outbit = value & 0x01;

	if (outbit)
		RF1_DTX = 1;
	else
		RF1_DTX = 0;
		
	while (RF1_DCLK == 0);
}
	
// Send 1 byte of data thru TRX
BYTE RF1_RXbyte()
{
	BYTE value = 0;
	
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x80;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x40;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x20;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x10;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x08;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x04;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x02;
		
	while (RF1_DCLK == 1);
	while (RF1_DCLK == 0);

	if (RF1_DRX)
		value |= 0x01;
		
	while (RF1_DCLK == 1);
}	

// Initialize TRX
void RF1_Init(int mode)
{
	RF1_RegisterSet(VCO_REG, 0x01C3, 0xF010);
	DelayMs(1);
	RF1_RegisterSet(TXRXCLK_REG, 0x2B14, 0x1560);
	RF1_RegisterSet(TEST_REG, 0x000E, 0x0000);	// TxRxCLK output for SPI

	if (mode == 1)
	{
		RF1_RegisterSet(N_REG, 0x5170, 0x4AF0);		// 906MHz, UART enabled
//		RF1_RegisterSet(N_REG, 0x5177, 0x9C20);		// 924MHz, UART enabled
		Delay10us(5);
		RF1_RegisterSet(TXMOD_REG, 0x010F, 0xF8D0);
	}
	else if (mode == 2)
	{
//		RF1_RegisterSet(IF_CAL_REG, 0xA649, 0x72B0);
//		RF1_RegisterSet(IF_SET_REG, 0x0000, 0x3140);
		DelayMs(6);
//		RF1_RegisterSet(SYNC_REG, PanID[0]);
		RF1_RegisterSet(N_REG, 0x5970, 0x4AF0);		// 906MHz, UART enabled
//		RF1_RegisterSet(N_REG, 0x5977, 0x9C20);		// 924MHz, UART enabled
		Delay10us(5);
		RF1_RegisterSet(DEMOD_REG, 0x81F1, 0x9210);
//		RF1_RegisterSet(AFC_REG, 0x2546, 0x3560);
	}
	else if (mode == 3)
	{
		RF1_RegisterSet(N_REG, 0x5170, 0x4AF0);		// 906MHz, UART enabled
//		RF1_RegisterSet(N_REG, 0x5177, 0x9C20);		// 924MHz, UART enabled
		Delay10us(5);
		RF1_RegisterSet(TXMOD_REG, 0x010F, 0xF8D0);
		RF1_RegisterSet(TEST_REG, 0x000E, 0x0400);	// test mode
	}
		
		
	TX_packet_length = 0;
	RX_read_index = 0;
	RX_write_index = 0;
}

// Change state of TRX
void RF1_SetState(BYTE state)
{
}

// Change TRX channel
void RF1_ChangeChannel(BYTE newChannel)
{
}

// Set a new Pan ID
void RF1_SetPanID(BYTE newID)
{
}

// Change TRX power level
void RF1_PowerLevel(BYTE newVal)
{
}

// Add a byte to the TX frame
void RF1_WriteData(BYTE newData)
{
	TX_frame[TX_packet_length++] = newData;
}

// Clear frame data
void RF1_ClearFrame(void)
{
	TX_packet_length = 0;
}
	
// Transmit a packet of data
void RF1_TxPacket(void)
{
	int i = 0;
	BYTE currState = TRX_state;		// save current state
	BYTE readState = BUSY_TX;
	
//	RF1_SetState(PLL_ON);			// set TX mode
	
	RF1_TXbyte(0xAA);				// preamble
	RF1_TXbyte(0xAA);
	RF1_TXbyte(0xAA);				// data
	RF1_TXbyte(0xAA);
	RF1_TXbyte(0xAA);
	RF1_TXbyte(0xAA);
	RF1_TXbyte(0xAA);
	RF1_DTX = 0;

//	RF1_SetState(currState);			// set previous TRX of state
}

// Check for RX message
int RF1_RXMessage(void)
{
	if (RX_read_index != RX_write_index)	// check for new message
		return 1;
	else
		return 0;
}

// Get RX buffer data
BYTE RF1_GetData(int index)
{
	return RX_buffer[RX_read_index][index];
}	

// Release packet
void RF1_ReleasePacket(void)
{
	if (RX_read_index < MAX_BUFFER)
		RX_read_index++;				// decrease buffer
	
	if (RX_read_index == RX_write_index)
	{
		RX_read_index = 0;				// reset pointers
		RX_write_index = 0;
	}	
		
}			

// INT1 Interrupt vector
void _ISRFAST __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
{
	int i = 0;
	BYTE temp = 0;
	
	RF1_IE = 0;							// disable further interrupts
		
	RF1_IF = 0;							// Clear flag
	RF1_IE = 1;							// re-enable interrupts
}
