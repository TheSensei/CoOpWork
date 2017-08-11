/*********************************************************************
 * AT86RF212.c
 * Written by Daniel W Harrist
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/
#include "AT86RF212.h"
#include "../../Microchip/Include/TimeDelay.h"
/************************ VARIABLES *********************************/


#ifndef TX_ONLY
BYTE RX_buffer[RX_BUFFER];   // fifo buffer
BYTE RX_drop_on_overrun;
WORD RX_read_offset;
WORD RX_write_offset;
#endif
BYTE TX_frame[FRAME_PAYLOAD_MAX_LENGTH];
BYTE TRX_state;
BYTE TX_packet_length;
BYTE TRX_error = TRX_NO_ERROR;

BYTE arrayKey[] = {0x5F, 0x1A, 0x65, 0x0C, 0x98, 0xA9, 0xC9, 0x6F,
                   0x49, 0xF6, 0xD3, 0x0A, 0x45, 0x6E, 0x7A, 0xC3,
                   0x2A, 0x27, 0x8C, 0x10, 0x20, 0x62, 0xE2, 0x6A,
                   0xE3, 0x48, 0xC5, 0xE6, 0xF3, 0x68, 0xA7, 0x04 };

// Disable interrrupts per section 8.5.4 of Pic24F family reffrence manual
int inline disable_radio_interrupts(void)
{
    int old_ipl;

    SET_AND_SAVE_CPU_IPL(old_ipl, RADIO_INTERRUPT_PRIORITY_LEVEL);

    return old_ipl;
}
void inline enable_radio_interrupts(int old_ipl)
{
    RESTORE_CPU_IPL(old_ipl);
}

// Write command to SPI bus
BYTE SPITransfer(BYTE v)
{
    BYTE i;
    WORD timeout = 0;

    IFS0bits.SPI1IF = 0;
    IFS0bits.SPF1IF = 0;
    i = SPI1BUF;
    SPI1BUF = v;
    while(IFS0bits.SPI1IF == 0 && IFS0bits.SPF1IF == 0)
    {
        timeout++;
        if (timeout > SPI_TO)
            break;
    }
    v = SPI1BUF;
    return v;
}

void SPIPut( BYTE v )
{
    (void) SPITransfer(v);
}

// Read byte from SPI bus
BYTE SPIGet(void)
{
    return SPITransfer(0x00);
}

// Set a register on the TRX
// Make sure interrupts are disabled before calling this
void TRX_RegisterSet(BYTE address, BYTE setting)
{
    int ipl;

    ipl = disable_radio_interrupts();
    TRX_CSn = 0;
    address = address | WRITE_REG;
    SPIPut(address);
    SPIPut(setting);
    TRX_CSn = 1;
    enable_radio_interrupts(ipl);
}

// Read a register on the TRX
// Make sure interrupts are disabled before calling this
BYTE TRX_RegisterRead(BYTE address)
{
    BYTE value;
    int ipl;

    ipl = disable_radio_interrupts();
    TRX_CSn = 0;
    address = address | READ_REG;
    SPIPut(address);
    value = SPIGet();
    TRX_CSn = 1;
    enable_radio_interrupts(ipl);

    return value;
}

// Read a register on the TRX
// Make sure interrupts are disabled before calling this
BYTE TRX2_RegisterRead(BYTE address)
{
    BYTE value;
    BYTE TRX2_IE_state = TRX2_IE;

    TRX2_IE = 0;
    TRX2_CSn = 0;
    address = address | READ_REG;
    SPIPut(address);
    value = SPIGet();
    TRX2_CSn = 1;
    TRX2_IE = TRX2_IE_state;

    return value;
}

// Reset TRX
void TRX_Reset(void)
{
    int interrupt_level;
    interrupt_level = disable_radio_interrupts();

    TRX_RSTn = 0;
    DelayMs(1);
    TRX_RSTn = 1;
    DelayMs(1);

    enable_radio_interrupts(interrupt_level);
}

// Initialize TRX
void TRX_Init(void)
{
    int interrupt_level;
    interrupt_level = disable_radio_interrupts();

    // initialize radio registers
    TRX_RegisterSet(TRX_CTRL_0, 0x00);          // disable CLKM
    TRX_RegisterSet(TRX_STATE, FORCE_TRX_OFF);  // set TRX_OFF state
    TRX_RegisterSet(IRQ_MASK, TRX_END);         // enable int for end of RX or TX
    TRX_RegisterSet(PHY_TX_PWR, MINUS_25);      // set output power - minimum
#if RADIO_REGION == North_America
    TRX_RegisterSet(TRX_CTRL_2, 0x94);          // set protocol - BPSK-40-ALT; enable buffer protection
#elif RADIO_REGION == United_Kingdom
    TRX_RegisterSet(TRX_CTRL_2,0x0);             // set protocol - BPSK-20
#else
    __error__ "Unsupported Radio Region"
#endif
    TRX_RegisterSet(CC_CTRL_1, 0x03);           // set RF band - 903-928
#if RADIO_REGION == North_America
    TRX_RegisterSet(CC_CTRL_0, CHAN_909);       // set RF freq
#elif RADIO_REGION == United_Kingdom
    TRX_RegisterSet(CC_CTRL_0, CHAN_915_9);
#else
    __error__ "Unsupported Radio Region"
#endif
    TRX_RegisterSet(SFD_VALUE, 0xB5);           // lock SFD across all devices

    TRX_state = TRX_OFF;
    TX_packet_length = 0;
#ifndef TX_ONLY
    RX_drop_on_overrun = 0;
    RX_read_offset = 0;
    RX_write_offset = 0;
#endif

    enable_radio_interrupts(interrupt_level);

    TRX_IF = 0;                                 // clear int flag
    TRX_IE = 1;                                 // enable int
}

// Calibrate TRX
BYTE TRX_Calibrate(void)
{
    DWORD timeout = 0;
    BYTE readVal = 0x80;

    if (!TRX_SetState(TRX_OFF))
        return 0;

    TRX_RegisterSet(FTN_CTRL, 0x80);            // start cal routine
    while ((readVal & 0x80))
    {
        Delay10us(1);
        readVal = TRX_RegisterRead(FTN_CTRL);
        timeout++;
        if (timeout > RADIO_TIMEOUT)
        {
            TRX_error = TRX_FAILED_CAL;
            return 0;
        }
    }

    if (!TRX_SetState(RX_ON))
        return 0;

    return 1;
}

// init AES engine
void TRX_InitAES(void)
{
    int interrupt_level;
    interrupt_level = disable_radio_interrupts();

    TRX_CSn = 0;                                // start SPI
    SPIPut(WRITE_SRAM);                         // write SRAM command
    SPIPut(AES_ADDRESS);                        // starting address
    SPIPut(0x40);                               // key command
    SPIPut(AES_KEY_1);
    SPIPut(AES_KEY_2);
    SPIPut(AES_KEY_3);
    SPIPut(AES_KEY_4);
    SPIPut(AES_KEY_5);
    SPIPut(AES_KEY_6);
    SPIPut(AES_KEY_7);
    SPIPut(AES_KEY_8);
    SPIPut(AES_KEY_9);
    SPIPut(AES_KEY_10);
    SPIPut(AES_KEY_11);
    SPIPut(AES_KEY_12);
    SPIPut(AES_KEY_13);
    SPIPut(AES_KEY_14);
    SPIPut(AES_KEY_15);
    SPIPut(AES_KEY_16);
    SPIPut(0x40);                               // key command again
    TRX_CSn = 1;                                // done with SPI

    enable_radio_interrupts(interrupt_level);
}

static inline BYTE
_getState()
{
    return TRX_RegisterRead(TRX_STATUS) & 0x1f;
}

// Change state of TRX
// Make sure interrupts are disabled before calling this
WORD TRX_SetState(BYTE state)
{
    DWORD timeout = 1;
    BYTE readState = _getState();

    while (readState != state) {
        if( state != STATE_TRANS )
            TRX_RegisterSet(TRX_STATE, state);          // set requested state
        Delay10us(1);
        readState = _getState();
        if (readState != state && timeout > RADIO_TIMEOUT)
        {
            TRX_error = TRX_FAILED_STATE_CHANGE;
            return 0;
        }
        timeout++;
    }

#ifdef TRX_FEM                                  // check if FEM installed
    if (state == RX_ON)                         // RX
    {
        TRX_ANTSEL = 0;                         // enable antenna
        TRX_TXEN = 0;
    }
    else if (state == PLL_ON || state == TX_START)  // TX
    {
        TRX_ANTSEL = 0;                         // enable antenna
        TRX_TXEN = 1;
    }
    else
    {
        TRX_ANTSEL = 1;                         // disable antenna
        TRX_TXEN = 0;
    }
#endif

    TRX_state = state;
    return timeout;
}

// Change TRX channel
void TRX_ChangeChannel(BYTE newChannel)
{
    int interrupt_level;
    interrupt_level = disable_radio_interrupts();

    TRX_RegisterSet(CC_CTRL_0, newChannel);     // RF frequency = 903 + (0.1 * CC_CTRL_0)

    enable_radio_interrupts(interrupt_level);
}

BYTE
TRX_dBmToPower( int power )
{
    if (power < MIN_TX_POWER)
        power = MIN_TX_POWER;
    if (power > MAX_TX_POWER)
        power = MAX_TX_POWER;
    switch(power) {
    case 11: return PLUS_11;
    case 10: return PLUS_10;
    case 9: return PLUS_9;
    case 8: return PLUS_8;
    case 7: return PLUS_7;
    case 6: return PLUS_6;
    case 5: return PLUS_5;
    case 4: return PLUS_4;
    case 3: return PLUS_3;
    case 2: return PLUS_2;
    case 1: return PLUS_1;
    case 0: return ZERO_DBM;
    case -1: return MINUS_1;
    case -2: return MINUS_2;
    case -3: return MINUS_3;
    case -4: return MINUS_4;
    case -5: return MINUS_5;
    case -6: return MINUS_6;
    case -7: return MINUS_7;
    case -8: return MINUS_8;
    case -9: return MINUS_9;
    case -10: return MINUS_10;
    case -11: return MINUS_11;
    case -12: return MINUS_12;
    case -13: return MINUS_13;
    case -14: return MINUS_14;
    case -15: return MINUS_15;
    case -16: return MINUS_16;
    case -17: return MINUS_17;
    case -18: return MINUS_18;
    case -19: return MINUS_19;
    case -20: return MINUS_20;
    case -21: return MINUS_21;
    case -22: return MINUS_22;
    case -23: return MINUS_23;
    case -24: return MINUS_24;
    default:
    case -25:
        return MINUS_25;
    }
}

// Change TRX power level
void TRX_PowerLevel(BYTE newVal)
{
    BYTE state = TRX_state;
    if (!TRX_SetState(TRX_OFF))
        return;

    if (newVal == PLUS_10)
        TRX_RegisterSet(RF_CTRL_0, 0x33);       // set PA offset to +2

    TRX_RegisterSet(PHY_TX_PWR, newVal);        // set output power

    TRX_SetState(state);
}

// Random Number Generator function of the radio. Section 11.2.1 At86rf212b
// radio manual.
// Only call this while radio is RX_ON or BUSY_RX
unsigned char TRX_GetTwoRandomBits(void)
{
    int interrupt_level = disable_radio_interrupts();
    unsigned char rssi_register = TRX_RegisterRead(PHY_RSSI);

    enable_radio_interrupts(interrupt_level);
    return (rssi_register & 0x60) >> 5;
}
unsigned char TRX_GetEightRandomBits(void)
{
    unsigned char bits = 0;
    int x = 0;
    for(x = 0; x < 4; x++)
    {
        bits = (bits << 2) + TRX_GetTwoRandomBits();
    }
    return bits;
}

#ifndef TX_ONLY
static void
_getRxFrameOverSpi(BYTE length, WORD offset)
{
    BYTE i;
    RX_buffer[offset] = length;
    offset = RX_BUFFER_OFFSET(offset + 1);
    for (i = 0; i < length; i++) {
        RX_buffer[offset] = SPIGet() ^ arrayKey[i % 32];
        offset = RX_BUFFER_OFFSET(offset + 1);
    }
    SPIGet(); // purge crc
    SPIGet();
    SPIGet(); // purge lqi
    RX_buffer[offset] = SPIGet(); // rssi
}
#endif

static void
_getRxFrame(void)
{
#ifndef TX_ONLY
    WORD next_offset, next_offset_limit;
    BYTE length;
#endif

    // Need this SPI write even if don't call SPIGet() below to clear radio rx frame when err
    SPIPut(READ_FRAME);

#ifndef TX_ONLY
    length = SPIGet() - 2;    // first byte is "phr" field (frame length, strip off crc length)
    if (length > FRAME_PAYLOAD_MAX_LENGTH) {
        TRX_error = TRX_RX_FRAME_TOO_LARGE;
        return;
    }

    next_offset_limit = RX_write_offset + RX_LENGTH_SIZE + length + RX_RSSI_SIZE;
    next_offset = RX_BUFFER_OFFSET(next_offset_limit);

    if (RX_drop_on_overrun && RX_write_offset != RX_read_offset &&
        (next_offset_limit > RX_read_offset || next_offset > RX_read_offset)) {
        TRX_error = TRX_DROP_NEWEST_APP_RX_BUF;
        return;
    }

    // Radio Rx frame good:
    _getRxFrameOverSpi(length, RX_write_offset);

    TRX_error = TRX_NO_ERROR;

    if( RX_write_offset != RX_read_offset ) {
      WORD read_offset_limit = RX_read_offset;
      while( next_offset_limit > read_offset_limit ||
             next_offset > RX_read_offset )
      {
          // Throw away oldest Rx buffer to make room for newest rx frame
          // else Rx buffer indexes equal which make buffer look empty
          read_offset_limit += RX_buffer[RX_read_offset] + 2;
          RX_read_offset = RX_BUFFER_OFFSET(read_offset_limit);
          TRX_error = TRX_DROP_OLDEST_APP_RX_BUF;
      }
    }
    RX_write_offset = next_offset;
#endif
}


static void
_receive( void )
{
    if ((TRX_RegisterRead(PHY_RSSI) & PHY_RSSI_RX_CRC_VALID) == 0)
    {
        TRX_error = TRX_RX_CRC_ERR;
        return;
    }

    /* does not require interrupt protection, occurs only through isr */
    TRX_CSn = 0;        // activate Radio chip's chip select
    _getRxFrame();
    TRX_CSn = 1;        // deactivate chip select
}

// INT2 Interrupt vector
void _ISRFAST __attribute__((interrupt, auto_psv)) _INT2Interrupt(void)
{
    TRX_IF = 0;   // Clear flag

    if (TRX_state == RX_ON)  // RX mode
        _receive();
    else
        TRX_SetState(RX_ON);

    TRX_RegisterRead(IRQ_STATUS);               // clear IRQ on TRX
}

// Add a byte to the TX frame
void TRX_WriteData(BYTE newData)
{
    TX_frame[TX_packet_length++] = newData;
}

// Clear frame data
void TRX_ClearFrame(void)
{
    TX_packet_length = 0;
}

// Transmit a packet of data
BYTE TRX_TxPacket(void)
{
    WORD i = 0;
    WORD stateChangeDelay = 0;
    BYTE state;
    int ipl;

    stateChangeDelay = TRX_SetState(PLL_ON);    // set TX mode
    if (stateChangeDelay == 0)                  // device didn't change state
    {
        TRX_error = TRX_FAILED_TX;
        return 0;                               // need to reset
    }

    ipl = disable_radio_interrupts();
    TRX_CSn = 0;                                // start SPI
    SPIPut(WRITE_FRAME);                        // write frame command
    SPIPut(TX_packet_length + 2);                   // PHR = frame length, max it can be 128 bytes
    for (i = 0; i < TX_packet_length; i++)
        SPIPut(TX_frame[i] ^ arrayKey[i % 32]);
    SPIPut(0); // provide 0 crc
    SPIPut(0);

    TRX_CSn = 1;                                // end of SPI
    enable_radio_interrupts(ipl);

    Delay10us(5);

    TRX_RegisterSet(TRX_STATE, TX_START);       // start TX

    state = BUSY_TX;

    while (TRX_state == PLL_ON && state == BUSY_TX)
    {
        Delay10us(5);
        if (TRX_state == PLL_ON) {
            BYTE currentState = _getState();
            if (currentState != STATE_TRANS)
                state = currentState;
        }
    }

    if (TRX_state != RX_ON)
    {
        TRX_RegisterRead(IRQ_STATUS);
        TRX_SetState(RX_ON);
    }

    return 1;
}

#ifndef TX_ONLY
/* after a call to TRX_RXMessage which returns true,
 * TRX_ReleasePacket must be called */
// Check for RX message, return in pointer
int inline TRX_RXMessage(void)
{
    return RX_drop_on_overrun = RX_read_offset != RX_write_offset;        // check for new message
}

// Get RX buffer length
BYTE TRX_GetLength()
{
    return RX_buffer[RX_read_offset];
}

// Get RX buffer data
BYTE TRX_GetData(int index)
{
    return RX_buffer[RX_BUFFER_OFFSET(RX_read_offset + RX_LENGTH_SIZE + index)];
}

// Get RX RSSI
BYTE TRX_GetRSSI(void)
{
    return RX_buffer[RX_BUFFER_OFFSET(RX_read_offset + RX_LENGTH_SIZE + RX_buffer[RX_read_offset])];
}

// Release packet
void TRX_ReleasePacket(void)
{
    RX_read_offset = RX_BUFFER_OFFSET(RX_read_offset + RX_LENGTH_SIZE + RX_buffer[RX_read_offset] + RX_RSSI_SIZE);
    RX_drop_on_overrun = 0;
}
#endif

BYTE TRX_CheckError(void)
{
    BYTE temp = TRX_error;                      // read error code
    TRX_error = TRX_NO_ERROR;                   // clear current error
    return temp;                                // return read error code
}
