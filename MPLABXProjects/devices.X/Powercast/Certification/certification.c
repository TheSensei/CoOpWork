/******************************************************************************
 * File:   main.c
 * Author: knolish
 * Copyright 2017 Powercast Inc.
 *
 * Created on April 3, 2017, 3:04 PM
 */


#include "../Common/AT86RF212.h"
#include "HardwareProfile.h"
#include "../../Microchip/Include/TimeDelay.h"
#include "../../Microchip/Include/DEE Emulation 16-bit.h"

volatile WORD proceed = 0;

const unsigned char whiten_array[] = {
    0x5f,0x1a,0x65,0x0c,0x98,0xa9,0xc9,0x6f,
    0x49,0xf6,0xd3,0x0a,0x45,0x6e,0x7a,0xc3,
    0x2a,0x27,0x8c,0x10,0x20,0x62,0xe2,0x6a,
    0xe3,0x48,0xc5,0xe6,0xf3,0x68,0xa7,0x04
};

const unsigned char packet_data[] = {
    0x00,0xaa,0x55,0xaa,0x55,0xaa,0x55,0x00,
    0x00,0x64,0x00,0xaa,0x55,0xaa,0x55,0xaa,
    0x55,0x00,0x00,0xaa,0x55,0xaa,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0xaa,0x55,0xaa,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void
write_frame(const unsigned char *packet, unsigned int packet_len)
{
    int i;
    int ipl = disable_radio_interrupts();

    /* Start SPI*/
    TRX_CSn = 0;
    SPIPut(WRITE_FRAME);
    /* Length of packet + 2 bytes for trailing CRC */
    SPIPut(packet_len + 2);
    for(i=0;i<packet_len;++i) {
        SPIPut(packet[i] ^ whiten_array[i & 0x1f]);
    }
    /* Trailing CRC of 0 */
    SPIPut(0);
    SPIPut(0);

    /* END SPI */
    TRX_CSn = 1;
    enable_radio_interrupts(ipl);

    Delay10us(5);
}

void
start_continuous_prbs_transmission(unsigned char channel,
                                   unsigned char power,
                                   unsigned char band,
                                   unsigned char modulation)
{
    TRX_Reset();
    TRX_IE = 0;

#if HAS_FEM == 1
    TRX_ANTSEL = 0;
    TRX_TXEN = 1;
#endif

    TRX_RegisterSet(IRQ_MASK,0x1);
    TRX_RegisterSet(TRX_STATE,0x03);
    TRX_RegisterSet(TRX_CTRL_0,0x0);

    /* Set cert channel and power */
    TRX_RegisterSet(CC_CTRL_0,channel);
    TRX_RegisterSet(PHY_TX_PWR,power);
    TRX_RegisterSet(CC_CTRL_1,band);

    if (TRX_RegisterRead(TRX_STATUS) != 0x08) {
        TRX_Reset();
        return;
    }

    /* test control register magic value*/
    TRX_RegisterSet(TST_CTRL_DIG,0xf);

    /* set the modulation */
    TRX_RegisterSet(TRX_CTRL_2,modulation);

    write_frame(packet_data,sizeof(packet_data));

    TRX_RegisterSet(PART_NUM,0x54);
    TRX_RegisterSet(PART_NUM,0x46);
    TRX_RegisterSet(TRX_STATE,0x09);

    DelayMs(20);

    TRX_RegisterSet(TRX_STATE,0x02);
}

void
start_continuous_cw_transmission(unsigned char channel,
                                 unsigned char power,
                                 unsigned char band,
                                 unsigned char modulation /* ignored */ )
{
    TRX_Reset();
    TRX_IE = 0;

    TRX_RegisterSet(IRQ_MASK,0x1); /* 0x0E <- 0x1 */
    TRX_RegisterSet(TRX_STATE,0x03); /* 0x02 <- 0x3 (FORCE_TRX_OFF) */
    TRX_RegisterSet(TRX_CTRL_0,0x00);

    /* Set cert channel and power */
    TRX_RegisterSet(PHY_TX_PWR,power);    /* 0x05 <- PLUS_5 */
    TRX_RegisterSet(RF_CTRL_0,0x03);       /* GR_TX_OFFSET = 3 */
    TRX_RegisterSet(CC_CTRL_1,band);       /* 0x14 <- 0x3 */
    TRX_RegisterSet(CC_CTRL_0,channel); /* 0x13 <- 915.9 */

    if (TRX_RegisterRead(TRX_STATUS) != 0x08) {  /* 0x01 */
        /* TRX_Reset(); */
        return;
    }

    /* test control register magic value*/
    TRX_RegisterSet(TST_CTRL_DIG,0xf);   /* 0x36 <- 0xf */
    TRX_RegisterSet(PART_NUM,0x54);      /* 0x1C <- 0x54 */
    TRX_RegisterSet(PART_NUM,0x42);      /* 0x1C <- 0x42 */
    TRX_RegisterSet(0x34,0x00);          /* 0x34 <- 0x00 */
    TRX_RegisterSet(0x3F,0x08);           /* 0x3F <- 0x08 */
    TRX_RegisterSet(TRX_STATE,0x09);     /* 0x02 <- 0x09 (PLL_ON) */

    DelayMs(20);

    TRX_RegisterSet(TRX_STATE,0x02);  /* 0x01 <- 0x02 (TX_START) */
}

void
stop_continuous_transmission()
{
    TRX_RegisterSet(PART_NUM,0x00);
    TRX_Reset();
}

/* Parameters stored in EEPROM for Connectors
 * Offset 0, 1 byte channel value
*/

unsigned char channel_addr = 0;

int
main(void)
{
    unsigned char channel;

    BoardInit();

#ifdef BUTTON
    if (RCONbits.POR == 1) {
        while (START_BTN == 0) {
            Sleep();
        }

        DelayMs(100);

        while (START_BTN == 1) {
            Sleep();
        }
    }
    channel = CERT_CHAN_LOW;
#else
    DataEEInit();
    dataEEFlags.val = 0;
    channel = DataEERead(channel_addr);
    if ( channel == 0 || channel == 0xFF) {
        channel = CERT_CHAN_LOW;
    }
#endif

    RCONbits.POR = 0;
#ifdef POWER_CONTROL
    Power(POWER_UP);
    DelayMs(3);
#endif

    /* TRX_Init(); */

    while (1) {
        start_continuous_prbs_transmission(channel,CERT_POWER,CERT_BAND,MODULATION);

        if (channel == CERT_CHAN_LOW) {
            channel = CERT_CHAN_MED;
        } else if (channel == CERT_CHAN_MED) {
            channel = CERT_CHAN_HIGH;
        } else {
            channel = CERT_CHAN_LOW;
        }
#ifndef BUTTON
        DataEEWrite(channel,channel_addr);
#endif

        while(1) {
            ClrWdt();
#ifdef HAS_LED
            LED = 1;
            DelayMs(300);
            ClrWdt();
            LED = 0;
#endif
            DelayMs(300);
#ifdef BUTTON
            if (START_BTN == 1) {
                while (START_BTN == 1) {
                    Sleep();
                }
                break;
            }
#endif
        }
        stop_continuous_transmission();
    }

    return 0;
}
