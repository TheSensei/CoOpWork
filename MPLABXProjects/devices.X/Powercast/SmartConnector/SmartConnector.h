/*********************************************************************
 * Smart Connector.h
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#ifndef SMART_CONNECTOR_H
#define	SMART_CONNECTOR_H

#include "AT86RF212.h"
#include "stdbool.h"

#define CONNECTOR_TX_FRAME  116
#define MAX_TX_BUFFERS       16  // was 20, but running into stack size problems
#define SIZE_CONNECTOR_HEADER_BYTES 10
#define SIZE_RADIO_TRAILER_BYTES 10

extern BYTE TX_rssi[MAX_TX_BUFFERS];
extern BYTE TX_ttl[MAX_TX_BUFFERS];
extern BYTE TX_low_power[(MAX_TX_BUFFERS + 7) / 8];
extern WORD rand_int;
extern DWORD myPacketCounter;
extern WORD TX_delay[MAX_TX_BUFFERS];
extern BYTE TX_write_index;
extern BYTE TX_read_index;
extern BYTE TX_buffer[MAX_TX_BUFFERS][CONNECTOR_TX_FRAME];
extern BYTE RX_packet[FRAME_PAYLOAD_MAX_LENGTH];
extern bool flashLight;
extern DWORD myNodeID;

#define MY_DEV_TYPE (myNodeID >> 24)

#endif	/* SMART_CONNECTOR_H */
