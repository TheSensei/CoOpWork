#include "Si4461.h"

/************************ VARIABLES *********************************/
BYTE TX_frame[TX_FRAME];
BYTE RX_buffer[MAX_BUFFER][RX_FRAME];		// fifo buffer
BYTE TRX_state = 0;
int TX_packet_length;
int RX_int;
int RX_read_index;
int RX_write_index;
BYTE PanID[26] = {0xA7, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 
				0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x1A, 0x2B, 0x3C,
				0x4D, 0x5E, 0x6F, 0x70, 0x81, 0x92, 0x11};

// Write command to SPI bus
void SPIPut(BYTE v)
{
    BYTE i;
    unsigned int timeout = 0;
	
	IFS0bits.SPI1IF = 0;
	IFS0bits.SPF1IF = 0;
	i = SPI1BUF;
	SPI1BUF = v;
	while(IFS0bits.SPI1IF == 0 && IFS0bits.SPF1IF == 0)
	{
    	timeout++; 
        if (timeout == SPI_TO) 
        	break;
    }
}

// Read byte from SPI bus
BYTE SPIGet(void)
{
	SPIPut(0x00);
	return SPI1BUF;
}

// Send one command byte to radio
void RF_SendCommand(int radio, BYTE command)
{
	BYTE RF1_IE_state = RF1_IE;
	BYTE RF2_IE_state = RF2_IE;

	if (radio == 1)
	{
		RF1_IE = 0;
		RF1_CSn = 0;
		SPIPut(command);
		RF1_CSn = 1;
		RF1_IE = RF1_IE_state;
	}
	else if (radio == 2)
	{
		RF2_IE = 0;
		RF2_CSn = 0;
		SPIPut(command);
		RF2_CSn = 1;
		RF2_IE = RF2_IE_state;
	}
			
}

// Send CTS command byte, read response, repeat until 0xFF
void RF_WaitCTS(int radio)
{
	BYTE RF1_IE_state = RF1_IE;
	BYTE RF2_IE_state = RF2_IE;
	BYTE CTS_check = 0;

	while (CTS_check != 0xFF)
	{
		if (radio == 1)
		{
			RF1_IE = 0;
			RF1_CSn = 0;
			SPIPut(READ_CMD_BUFF);
			CTS_check = SPIGet();
			RF1_CSn = 1;
			RF1_IE = RF1_IE_state;
		}
		else if (radio == 2)
		{
			RF2_IE = 0;
			RF2_CSn = 0;
			SPIPut(READ_CMD_BUFF);
			CTS_check = SPIGet();
			RF2_CSn = 1;
			RF2_IE = RF2_IE_state;
		}
		
		Delay10us(1);
	}
}

// Set a register on the TRX
void RF_RegisterSet(BYTE address, BYTE setting)
{
	BYTE RF1_IE_state = RF1_IE;
        
	RF1_IE = 0;
	RF1_CSn = 0;
	address = address | WRITE_REG;
	SPIPut(address);
	SPIPut(setting);
	RF1_CSn = 1;
	RF1_IE = RF1_IE_state;
}
    
// Read a register on the TRX
BYTE RF_RegisterRead(BYTE address)
{
	BYTE value;
	BYTE RF1_IE_state = RF1_IE;
        
	RF1_IE = 0;
	RF1_CSn = 0;
	address = address | READ_REG;
	SPIPut(address);
	value = SPIGet();
	RF1_CSn = 1;
	RF1_IE = RF1_IE_state;

	return value;
}
    
// Initialize TRX
void RF_Init(int radio)
{
	if (radio == 1)
	{
		RF1_SDN = 0;
		DelayMs(5);
	}	
	else if (radio == 2)
	{
		RF2_SDN = 0;
		DelayMs(5);
	}	
		
	RF_SendCommand(radio, POWER_UP);
	RF_WaitCTS(radio);
		
		
	
	TRX_state = TRX_OFF;
	TX_packet_length = 0;
	RX_int = 0;
	RX_read_index = 0;
	RX_write_index = 0;
}

// Change state of TRX
void RF_SetState(BYTE state)
{
}

// Change TRX channel
void RF_ChangeChannel(BYTE newChannel)
{
}

// Set a new Pan ID
void RF_SetPanID(BYTE newID)
{
}

// Change TRX power level
void RF_PowerLevel(BYTE newVal)
{
}

// INT1 Interrupt vector
void _ISRFAST __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
{
	int i = 0;
	BYTE temp = 0;
	
	RF1_IE = 0;							// disable further interrupts
	
	if (TRX_state == RX_ON && RX_write_index < MAX_BUFFER)	// RX mode, buffer not full
	{
		
		if (temp & 0x80)
		{
			RF1_CSn = 0;					// start SPI 
			SPIPut(READ_FRAME);				// read command to TRX
			RX_buffer[RX_write_index][0] = SPIGet();	// first byte is frame length
			for (i = 1; i < (RX_buffer[RX_write_index][0] + 3); i++)
			{
				RX_buffer[RX_write_index][i] = SPIGet();
			}	
				
			RF1_CSn = 1;					// end of SPI
			
			for (i = RX_buffer[RX_write_index][0] + 4; i < RX_FRAME; i++)
				RX_buffer[RX_write_index][i] = 0;			// zero empty packet space
		
			RX_write_index++;				// increase buffer
		}
	}
		
	RF1_IF = 0;							// Clear flag
	RF1_IE = 1;							// re-enable interrupts
}

// INT2 Interrupt vector
void _ISRFAST __attribute__((interrupt, auto_psv)) _INT2Interrupt(void)
{
	int i = 0;
	BYTE temp = 0;
	
	RF2_IE = 0;							// disable further interrupts
	
	if (TRX_state == RX_ON && RX_write_index < MAX_BUFFER)	// RX mode, buffer not full
	{
		
		if (temp & 0x80)
		{
			RF1_CSn = 0;					// start SPI 
			RX_buffer[RX_write_index][0] = SPIGet();	// first byte is frame length
			for (i = 1; i < (RX_buffer[RX_write_index][0] + 3); i++)
			{
				RX_buffer[RX_write_index][i] = SPIGet();
			}	
				
			RF1_CSn = 1;					// end of SPI
			
			for (i = RX_buffer[RX_write_index][0] + 4; i < RX_FRAME; i++)
				RX_buffer[RX_write_index][i] = 0;			// zero empty packet space
		
			RX_write_index++;				// increase buffer
		}
	}
		

	RF2_IF = 0;							// Clear flag
	RF2_IE = 1;							// re-enable interrupts
}
    
// Add a byte to the TX frame
void RF_WriteData(BYTE newData)
{
	TX_frame[TX_packet_length++] = newData;
}

// Clear frame data
void RF_ClearFrame(void)
{
	TX_packet_length = 0;
}
	
// Transmit a packet of data
void RF_TxPacket(void)
{
	int i = 0;
	BYTE currState = TRX_state;		// save current state
	BYTE readState = BUSY_TX;
	
	RF_SetState(PLL_ON);			// set TX mode

	RF1_IE = 0;						// disable TRX int
	RF1_CSn = 0;					// start SPI 
	SPIPut(WRITE_FRAME);			// write frame command
   	SPIPut(TX_packet_length + 2);	// PHR = frame length, max it can be 128 bytes
   	for (i = 0; i < TX_packet_length; i++)
   	   	SPIPut(TX_frame[i]);
	
	SPIPut(0x00);					// extra byte for CRC
	SPIPut(0x00);					// extra byte for CRC
	RF1_CSn = 1;					// end of SPI
	RF1_IE = 1;						// enable TRX int
	
	RF_SetState(TX_START);			// set TX mode

	while (readState != PLL_ON)		// wait for IRQ from TRX
	{
		Delay10us(1);
	}	

	RF_SetState(currState);			// set previous TRX of state
}

// Check for RX message, return in pointer
int RF_RXMessage(void)
{
	if (RX_read_index != RX_write_index)	// check for new message
		return 1;
	else
		return 0;
}

// Get RX buffer data
BYTE RF_GetData(int index)
{
	return RX_buffer[RX_read_index][index];
}	

// Release packet
void RF_ReleasePacket(void)
{
	if (RX_read_index < MAX_BUFFER)
		RX_read_index++;				// decrease buffer
	
	if (RX_read_index == RX_write_index)
	{
		RX_read_index = 0;				// reset pointers
		RX_write_index = 0;
	}	
		
}			
