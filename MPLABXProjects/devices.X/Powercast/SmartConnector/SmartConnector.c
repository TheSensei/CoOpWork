/*********************************************************************
 * Smart Connector.c
 * Written by Daniel W Harrist
 * Updated by Mark Brasili
 *   - Added f/w upgrade support
 *   - Added diagnostic support
 *   - Added run-time support for other connector types (e.g., Sparrow & Emerge)
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

//************************ HEADERS ****************************************
#include "HardwareProfile.h"
#include "DEE Emulation 16-bit.h"
#include "../Common/AT86RF212.h"
#include "../Common/RadioUtils.h"
#include <stdlib.h>
#include <math.h>
#include "flash_functions.h"
#include "version.h"
#include "fw_update_req_handlers.h"
#include "../Common/diag_functions.h"
#include "tx_functions.h"
#include "device_type_functions.h"
#include "SmartConnector.h"


// TO DO - maybe clean-up older request handlers code

// Smart Connector Commands
#define TURN_OFF            10
#define SCENE_1             15
#define TURN_ON             20
#define SCENE_2             25
#define INCREASE_DIM        30
#define SCENE_3             35
#define DECREASE_DIM        40
#define SCENE_4             45
#define SET_LIGHT_LEVEL     50
#define SET_SCENE           51
#define REQUEST_STATE       100
#define CONNECTOR_RESPONSE  101
#define IS_GATEWAY_REQUEST  102
#define IS_GATEWAY          103
#define ADD_DEVICE          110
#define UPDATE_DEVICE_MOD   112
#define UPDATE_DEVICE_FADE  114
#define UPDATE_DEVICE_ON_ACTION  116
#define UPDATE_DEVICE_SCENE_SELECT 118
#define REMOVE_DEVICE       120
#define REMOVE_ALL_DEVICES  130
#define RESET_CONNECTOR     131
#define UPDATE_CONNECTOR    200
#define UPDATE_CONNECTOR_SCENE 205
#define UPDATE_CONNECTOR_POSTSCALER 207
#define SIZE_CONNECTOR_RESPONSE 37
#define SIZE_IS_GATEWAY         10

#define MAX_DEVICES         32
#define BIND_TO             40000

#define GATEWAY_DISASSOCIATE_TIMEOUT (60 * 60 * 6)

#define P_KERMIT 0x8408

#define FADE_VALUE 0x3f
#define FADE_MINUTES 0x80
#define FADE_PERIOD 26 /* 156.25 / 6 */

#define MAX_SCENE 16
#define MAX_BUTTON 4

struct AssociatedDevice {
  DWORD id;
  DWORD packetCounter:24;
  BYTE mod;
  BYTE fade;
  BYTE on_action;
  BYTE button_scene[MAX_BUTTON];
};

//************************** VARIABLES ************************************
__prog__ volatile DWORD mfgNodeID __attribute__((space(prog),address(0xABF0)));
DWORD myNodeID;
//#define TX_UNSOLICITED_ERROR_MESSAGE_TO_GW

BYTE RX_packet[FRAME_PAYLOAD_MAX_LENGTH];
BYTE TX_buffer[MAX_TX_BUFFERS][CONNECTOR_TX_FRAME];
WORD TX_delay[MAX_TX_BUFFERS];
BYTE TX_rssi[MAX_TX_BUFFERS];
BYTE TX_ttl[MAX_TX_BUFFERS];
BYTE TX_low_power[(MAX_TX_BUFFERS + 7) / 8];
WORD TX_delay_counter = 0;
BYTE TX_read_index = 0;
BYTE TX_write_index = 0;
DWORD recNodeID = 0;
DWORD recPacketCounter = 0;
WORD radioResetTimer = 0;
WORD radioResetLimit = 600;
WORD radioCalTimer = 0;
WORD radioCalLimit = 300;
bool doCommand = false;
WORD command = 0;
DWORD lastNodeID[WINDOW_SIZE];
DWORD lastPacketCounter[WINDOW_SIZE];
WORD lastPacketUpdate = 0;
DWORD myPacketCounter = 10;
DWORD myRecPacketCounter = 0;
BYTE statePostscaler = 100;
BYTE channel = 1;
WORD rand_int = 0;
bool repeater = 0;
DWORD gatewayID = 0;
BYTE deviceCount = 0;
WORD deviceChecksum = 0;
WORD deviceModChecksum = 0;
WORD deviceFadeChecksum = 0;
WORD deviceOnActionChecksum = 0;
WORD connectorSceneChecksum = 0;
WORD deviceSceneSwitchChecksum = 0;
struct AssociatedDevice associatedDevices[MAX_DEVICES];

DWORD spoofPktCntr = 0;
WORD relayOutputLevel = 600; // target, pre relay rising
WORD activeOutputLevel = 600; // actual dimming output, including relay rising
WORD newOutputLevel = 600;
WORD defaultSetpoint = 0;    // Unoccupied State

// Dimmer Inversion Control
//
// This boolean controls the dimmer inversion logic. The dimmer inversion logic
// is applied when the connector is driven to a state of 0, that is turned off.
// false = normal dim, true = invert dim
//
// Normal behavior for a line dimming connector is to drive the dimming logic
// to zero and to drive the relay off.  Inverted behavior is to drive the
// dimming logic full on and to energize the relay, essentially turning the
// connector full on.  The use case is, for say a hallway, to turn off all
// of the fixtures, except leave every 6th fixture, say, on as a night-light.
//
// Normal behavior for a non-line dimming connector is to drive the dimming
// logic full on while driving the relay off.  Thus all fixtures are turned
// off, except for fixtures powered independently by a source other than the
// connector's relay.  These emergency fixtures remain on.  Inverted behavior
// is to drive the dimming logic to 0, thus forcing all connectors off, even
// those whose power source is not controlled by the connector.

bool invertDimIfNewStateZero = false;
bool invertDimChanged = false;

BYTE occSetpoint = 50;
bool occupiedState = false;
bool repeat = false;
bool updEEdevices = false;
bool updEEstatus = false;
BYTE scene[MAX_SCENE];
bool connectorHasBeenPolled = false;
WORD startupHeartbeatTimerLimit = 600;
WORD startupHeartbeatTimer = 600;
BYTE offDimLevelCounter = 2;

// after 6 hours with no contact from an associated gateway, we detach
WORD gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;

DWORD fadeAfterTime = 0;
DWORD fadeTickCount = 0;
WORD fadeLastTime = 0;
BYTE fadeToState = 100;


#define TX_HIGH (FEM_POWER)
#define TX_LOW (-20)
        
CHAR txPower = TX_HIGH;     /* dBm */
CHAR txLowPower = TX_LOW; /* dBm */

// variables used by local vacancy connectors a.k.a "room-in-a-box"
WORD vacancyTimer = 0;  // restart count-up timer
WORD vacancyMaxSeconds = 25*60;   // 25 Minutes to declare room unoccupied

WORD packetRssi = 0;

// dimming not used by Sparrow/Outlet connector
BYTE maxSetpoint = 100;
BYTE minSetpoint = 0;

// flashlight not used by Sparrow/Outlet connector
bool flashLight = false;
bool flashLightStart = false;
bool flashLightDone = false;
WORD flashLightTimer = 0;
WORD relayOutputLevelSave;

WORD startupDimDelay = 0;  // used by Emerge Slower PWM connector

bool stateChangeDone = false;
BYTE errorStatus = 0;

DWORD rxedGatewayID = 0;

BYTE relayRisingDelay = 0;
BYTE relayRisingLevel = 0;
BYTE relayRisingCountdown = 0;

// EEPROM
WORD readEEdata1 = 0;
WORD readEEdata2 = 0;

// Globals in fw_update_req_handlers.c
extern WORD fwUpgradeTimer;

// Macros Controlling the layout of the EEPROM

#define CHANNEL_AND_STATE_POSTSCALER_ADDR 0
#define REPEATER_ADDR                     (CHANNEL_AND_STATE_POSTSCALER_ADDR + 1)
#define GATEWAYID_ADDR                    (REPEATER_ADDR + 1)
#define DEFAULT_SET_POINT_ADDR            (GATEWAYID_ADDR + 2)
#define DEVICECOUNT_AND_TX_POWER_ADDR     (DEFAULT_SET_POINT_ADDR + 1)
#define ASSOCIATEDDEVICES_ADDR            (DEVICECOUNT_AND_TX_POWER_ADDR + 1)
#define PACKETCOUNTERS_ADDR               (ASSOCIATEDDEVICES_ADDR + (MAX_DEVICES * 4))

void InitalizeTheConnector();
WORD AssociatedDevicesChecksum();
void CalibrateEvery5Minutes();
void AddPollResponseToTXBuffer(WORD delay);
void CheckForReceivedDataPacket();
void AddPacketToTxBufferIfRepeater( WORD command, WORD frameLength );
void CheckTRXErrorStatus();
void CheckTheTxBufferForUnsentPackets();
void DoFlashCommand();  // this fnc not used by Sparrow/Outlet connectors
void UpdateOutputState();
void LoadNVRamData();
void UpdateNVMemory();
void UpdateNVDevices();
int inline disable_timer_interrupts(void);
void inline enable_timer_interrupts(int old_ipl);
void update_connector_checksums();

// Utility functions to manage the ownership of the connector by a
// gateway.  A gatewaID value of 0 means that the connector is
// currently unowned.  A gatewayID value of 0x00fffff0 means that the
// connector is in local vacancy mode and operates independently of
// the gateway.  Any other value means that the connector is owned by
// that gateway.
//
// Ownership Rules
//
//   1) An unowned connector may be claimed by any gateway
//
//   2) An owned connector shall not be managed by a gateway other
//   than the owning gateway.
//
//   3) Ownership expires after 6 hours of no communication with the
//   owning gateway.  The gateway becomes unowned, but otherwise
//   continues to operate as configured
//
//   4) A gateway in local vacancy mode may be claimed by any other
//   gateway as if it was unowned.

bool inline is_legal_gw_id(DWORD gatewayID);
bool inline is_local_vacancy_gw_id(DWORD gatewayID);
bool inline is_managed_connector();
bool inline is_unowned();
bool inline is_local_vacancy_connector();

/*********************************************************************
* Main
**********************************************************************/
int main(void)
{
#ifdef PROBED_CONNECTOR
    // Variables to help instrument the main loop performance
    // This causes p22 to output a square wave whose period
    // is 200 times through the main loop
    int p22_limit = 100;
    int p22_state = 0;
    int p22_incr = 1;
#endif

    reqFwReset();       // Ensure any in-progress upgrades are cancelled
    flash_boot_init();  // flash used for f/w download

    InitalizeTheConnector();

    // The connector starts out energized, so vacacancy devices need to behave as
    // if the room is occupied.
    if(isLocalVacancyConn(MY_DEV_TYPE))
    {
        occupiedState = true;
    }

#ifdef PROBED_CONNECTOR
    P22 = 0;
#endif
    // start continuous loop
    while (1)
    {
#ifdef PROBED_CONNECTOR

        if (p22_state <= 0){
            P22 = 1;
            p22_incr = 1;
        } else if (p22_state >= p22_limit ){
            P22 = 0;
            p22_incr = -1;
        }
        p22_state = p22_state + p22_incr;
#endif

        // kick the dog
        ClrWdt();

        // Reset the Radios if required
        if( radioCalTimer >= radioCalLimit || radioResetTimer >= radioResetLimit)
        {
            disable_radio_interrupts();
            CalibrateEvery5Minutes();
            enable_radio_interrupts(0);
        }

        if (!connectorHasBeenPolled && startupHeartbeatTimer > startupHeartbeatTimerLimit)
        {
            if (is_managed_connector())
                AddPollResponseToTXBuffer(0);
            startupHeartbeatTimer -= 600;
        }

        // Check Packets
        if(TRX_RXMessage())
        {
            CheckForReceivedDataPacket();

            // Check if Radio Driver logged a Rx error
            CheckTRXErrorStatus();
        }

        // Process Packet if one exists in the buffer
        if (TX_read_index != TX_write_index)
        {
            // If we are not a bi-pin relay connector or if we are
            // a bi-pin connector and the relay is currently not energized,
            // transmit the packet.  Otherwise, do not run the transmitter
            // and the relay coil simultaneously for reasons of power
            // supply capacity.
            if (!isBiPinRelayConn(MY_DEV_TYPE) || T5CONbits.TON != 1 )
                CheckTheTxBufferForUnsentPackets();

            //Check if Radio Driver logged a Tx error
            CheckTRXErrorStatus();
        }

        disable_timer_interrupts();
        if (is_managed_connector() && gatewayDisassociateTimeout == 0)
        {
            gatewayID = 0;
            updEEstatus = true;
        }

        if(isLocalVacancyConn(MY_DEV_TYPE))
        {
            //*********************************************************************
            // Update vacancy status
            //*********************************************************************
            if (is_local_vacancy_connector() && occupiedState) // associated gatway will control vacancy
            {
                if (vacancyTimer > vacancyMaxSeconds)       // default 30 minutes
                {
                    vacancyTimer = 0;  //restart count-up timer
                    newOutputLevel = 0;
                    occupiedState = false;
                }
            }
            else
                vacancyTimer = 0;  // restart count-up timer
        }

        if(isFlashLightConn(MY_DEV_TYPE))
        {
            // Do 60 second flash
            if (flashLight)
                DoFlashCommand();
            else
            {
                flashLightStart = false;
                flashLightDone = false;
            }
        }

        if (isDimmingConn(MY_DEV_TYPE))
        {
            WORD fadeToOutputLevel = fadeToState * 6;
            if (!flashLight && fadeToOutputLevel != newOutputLevel)
            {
                /* wrap */
                if( TMR4 < fadeLastTime ) {
                    WORD tmp = (0xffff - fadeLastTime + 1) + TMR4;
                    fadeLastTime = 0;
                    TMR4 = tmp;
                }
                while( fadeLastTime < 0xffff - FADE_PERIOD + 1 &&
               TMR4 >= fadeLastTime + FADE_PERIOD ) {
                    fadeTickCount++;
                    fadeLastTime += FADE_PERIOD;
                }

                while( fadeTickCount >= fadeAfterTime &&
                       fadeToOutputLevel != newOutputLevel ) {
                    if( fadeToOutputLevel > newOutputLevel )
                        newOutputLevel++;
                    else
                        newOutputLevel--;
                    fadeTickCount -= fadeAfterTime;
                }
            }
        }

        // Change Relay State if different, provided we are not
        // already doing so (bi-stable relay), and not delaying
        // (Emerge slower PWM)
        if (activeOutputLevel != newOutputLevel && T5CONbits.TON != 1 && startupDimDelay != 0) {
            UpdateOutputState();
        }

	// Change relay state if we have changed the inversion setting
	if ( invertDimChanged && T5CONbits.TON != 1 && startupDimDelay != 0 ) {
	    UpdateOutputState();
	}
        enable_timer_interrupts(0);

        // Update NV Ram if needed
        if (updEEstatus)
            UpdateNVMemory();
        if (updEEdevices)
            UpdateNVDevices();
    }
}

void InitalizeTheConnector()
{
    int i = 0;

    myNodeID = mfgNodeID;

    srand(myNodeID);
    startupHeartbeatTimerLimit += rand() % 25;

    // Initialize the system
    BoardInit(MY_DEV_TYPE);

    // Get data from EEPROM
    DataEEInit();
    dataEEFlags.val = 0;
    LoadNVRamData();

    deviceChecksum = AssociatedDevicesChecksum();

    // Initialize buffers
    for (i = 0; i < WINDOW_SIZE; i++)
    {
        lastNodeID[i] = 0;
        lastPacketCounter[i] = 0;
    }

    // Initialize radio, set to receive
    while (!stateChangeDone)
    {
        TRX_Reset();
        TRX_Init();
        TRX_PowerLevel(TRX_dBmToPower(txPower));
        if (channel == 2)                               // check for channel change
            TRX_ChangeChannel(CHAN_921);

        if (TRX_SetState(RX_ON))                        // set radio to receive
            stateChangeDone = true;
        else
            CheckTRXErrorStatus();  // Check if Radio Driver logged a State Change error

        DelayMs(1);                                     // wait, then try again
    }

    // Generate a random number to use for the packet counter
    myPacketCounter = ((DWORD)TRX_GetEightRandomBits() << 16) +
                      ((DWORD)TRX_GetEightRandomBits() <<  8) +
                      (TRX_GetEightRandomBits());

    if (!isEmergeSlowerPwmConn(MY_DEV_TYPE))
        startupDimDelay = 1;  // leave at 0 for EM slower PWM conn for timer to set to 1

#ifdef USE_DIAG_TEST_CODE_AT_INIT_TIME
    test_code_load_counts_with_err_number();
#endif
}

void CalibrateEvery5Minutes()
{
    //*********************************************************************
    // Calibrate every 5 minutes
    //*********************************************************************
    if (radioCalTimer >= radioCalLimit)             // calibrate radios when limit is reached
    {
        radioCalTimer = 0;
        if (!TRX_Calibrate())
            radioResetTimer = radioResetLimit;      // force reset
        else
            CheckTRXErrorStatus();  // Check if Radio Driver logged a CAL error
    }

    //*********************************************************************
    // Reset the radio todo second Cal every 5 minutes
    //*********************************************************************
    if (radioResetTimer >= radioResetLimit)         // reset radios when limit is reached
    {
        radioResetTimer = 0;
        stateChangeDone = false;
        // Initialize radio, set to receive
        while (!stateChangeDone)
        {
            TRX_Reset();
            TRX_Init();
            TRX_PowerLevel(TRX_dBmToPower(txPower));
            if (channel == 2)                       // check for channel change
                TRX_ChangeChannel(CHAN_921);

            if (TRX_SetState(RX_ON))                // set radio to receive
                stateChangeDone = true;
            else
                CheckTRXErrorStatus();  // Check if Radio Driver logged a State Change error

            DelayMs(1);                             // wait, then try again
        }
    }
}

BYTE
RepeatOffset( BYTE nodeType, WORD command, BYTE *packet )
{
  if( nodeType == DEV_TYPE_LIGHT_SENSOR && command == 0 )
    return 1 + 12;

  switch(command) {
  case RESP_FW_INFO: return 1 + SIZE_RESP_FW_INFO;
  case RESP_FW_MISSING: return 1 + SIZE_RESP_FW_MISSING;
  case RESP_FW_MODE: return 1 + SIZE_RESP_FW_MODE;
  case RESP_DIAG_OP:
    return 1 + respDiag_length(&RX_packet[11]);
  case REQ_FW_UPGRADE: return 1 + SIZE_REQ_FW_UPGRADE;
  case REQ_FW_DATA_HEADER_OR_DATA:
    return 1 + ((RX_packet[11] & 0x40) ? SIZE_REQ_FW_DATA : SIZE_REQ_FW_DATA_HEADER);
  case REQ_FW_MISSING: return 1 + SIZE_REQ_FW_MISSING;
  case REQ_FW_APPLY: return 1 + SIZE_REQ_FW_APPLY;
  case REQ_DIAG_OP:
    return 1 + ((RX_packet[11] & REQ_DIAG_FLAGS_GW_BITMAP_PRESENT_BIT) ? SIZE_REQ_DIAG_WITH_GW_BITMAP : SIZE_REQ_DIAG_NO_GW_BITMAP);
  case CONNECTOR_RESPONSE:
    return 1 + SIZE_CONNECTOR_RESPONSE;
  case UPDATE_CONNECTOR:
    return 1 + 25;
  case UPDATE_CONNECTOR_SCENE:
      return 1 + 26;
  case ADD_DEVICE:
  case REMOVE_DEVICE:
    return 1 + 14;
  case UPDATE_DEVICE_MOD:
  case UPDATE_DEVICE_FADE:
  case UPDATE_DEVICE_ON_ACTION:
    return 1 + 15;
  case UPDATE_DEVICE_SCENE_SELECT:
      return 1 + 18;
  case SET_LIGHT_LEVEL:
    return 1 + 11;
  case SCENE_1:
  case SCENE_2:
  case SCENE_3:
  case SCENE_4:
    return 1 + 10;
  case UPDATE_CONNECTOR_POSTSCALER:
      return 1+11;
  case SET_SCENE:
    return 1 + 11;
  default: return 1 + 10;
  }
}

bool
HasFooter( WORD command, WORD frameLength, BYTE *packet, BYTE *outRepeatOffset )
{
    BYTE repeatOffset = *outRepeatOffset = RepeatOffset(packet[1], command, packet);
    return 1 + frameLength == repeatOffset + SIZE_RADIO_TRAILER_BYTES;
}

inline DWORD
RepeaterGateway( BYTE repeatOffset, BYTE *packet )
{
    return ((DWORD)packet[repeatOffset + 0] << 24) |
        ((DWORD)packet[repeatOffset + 1] << 16) |
        ((DWORD)packet[repeatOffset + 2] << 8) |
         (DWORD)packet[repeatOffset + 3];
}

WORD
crc_kermit_tabentry( BYTE i )
{
  WORD crc = 0, c = (WORD) i;
  BYTE j;

  for( j = 0; j < 8; j++ ) {
    if( (crc ^ c) & 0x0001 )
      crc = (crc >> 1) ^ P_KERMIT;
    else
      crc = crc >> 1;

    c = c >> 1;
  }

  return crc;
}

WORD
update_crc_kermit( WORD crc, BYTE c )
{
  WORD tmp, short_c;

  short_c = 0x00ff & (WORD) c;

  tmp = crc ^ short_c;
  crc = (crc >> 8) ^ crc_kermit_tabentry(tmp & 0xff);

  return crc;
}

WORD
AssociatedDevicesChecksum()
{
  BYTE i;
  WORD crc = 0;
  for( i = 0; i < deviceCount; i++ ) {
    crc = update_crc_kermit(crc, (associatedDevices[i].id >> 24) & 0xff);
    crc = update_crc_kermit(crc, (associatedDevices[i].id >> 16) & 0xff);
    crc = update_crc_kermit(crc, (associatedDevices[i].id >> 8) & 0xff);
    crc = update_crc_kermit(crc, (associatedDevices[i].id >> 0) & 0xff);
  }
  return ((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8);
}

WORD
AssociatedDevicesModChecksum()
{
  BYTE i;
  WORD crc = 0;
  for( i = 0; i < deviceCount; i++ )
    crc = update_crc_kermit(crc, associatedDevices[i].mod);
  return ((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8);
}

WORD
AssociatedDevicesFadeChecksum()
{
  BYTE i;
  WORD crc = 0;
  for( i = 0; i < deviceCount; i++ )
    crc = update_crc_kermit(crc, associatedDevices[i].fade);
  return ((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8);
}

WORD
AssociatedDevicesOnActionChecksum()
{
  BYTE i;
  WORD crc = 0;
  for( i = 0; i < deviceCount; i++ )
    crc = update_crc_kermit(crc, associatedDevices[i].on_action);
  return ((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8);
}

WORD
ConnectorSceneChecksum()
{
    BYTE i;
    WORD crc = 0;
    for(i=0;i<MAX_SCENE;++i) {
	crc = update_crc_kermit(crc,scene[i]);
    }
    return ((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8);
}

WORD
AssociatedDevicesSceneSelectChecksum()
{
    BYTE i;
    BYTE j;
    WORD crc = 0;
    for( i = 0;i< deviceCount; ++i) {
	for ( j = 0; j < MAX_BUTTON;++j ) {
	    crc = update_crc_kermit(crc,associatedDevices[i].button_scene[j]);
	}
    }
    return ((crc & 0xff00) >> 8 ) | ((crc & 0xff) << 8);
}

bool
LocalRepeater( DWORD nodeID )
{
  BYTE i, mod;
  for( i = 0; i < deviceCount && nodeID != associatedDevices[i].id; i++ );
  if( i == deviceCount )
    return false;
  mod = associatedDevices[i].mod;
  if( mod == 0 )
    return false;
  srand(TMR1 ^ myNodeID);
  return rand() % mod == 0;
}

bool
ShouldRepeatPacket( DWORD nodeID, bool hasFooter, DWORD pktGatewayID )
{
  if( !repeater && !LocalRepeater(nodeID) )
    return false;

  if( nodeID == myNodeID )
    return false;

  if( is_unowned() )
    return false;

  if( is_local_vacancy_gw_id(gatewayID)) {
      return false;
  }

  if( hasFooter && pktGatewayID != gatewayID )
    return false;

  if( fwUpgradeTimer > 0 && ((nodeID & 0xff000000) != (myNodeID & 0xff000000)) )
    return false;

  return true;
}

void AddPollResponseToTXBuffer(WORD delay)
{
    txRespPrep(delay);

    loadRespHeaderIntoTxBuff(CONNECTOR_RESPONSE, SIZE_CONNECTOR_RESPONSE);

    TX_buffer[TX_write_index][8] = (defaultSetpoint & 0x7f) | (invertDimIfNewStateZero ? 0x80 : 0);  // if inverted, or in 0x80 bit

    TX_buffer[TX_write_index][11] = ((repeater ? 1 : 0) << 2) | (channel & 0x03);

    TX_buffer[TX_write_index][12] = fadeToState;

    TX_buffer[TX_write_index][13] = deviceCount;

    TX_buffer[TX_write_index][14] = ((occupiedState ? 1 : 0) << 7) + occSetpoint;

    TX_buffer[TX_write_index][15] = maxSetpoint;

    TX_buffer[TX_write_index][16] = scene[0];
    TX_buffer[TX_write_index][17] = scene[1];
    TX_buffer[TX_write_index][18] = scene[2];
    TX_buffer[TX_write_index][19] = scene[3];

    TX_buffer[TX_write_index][20] = (deviceChecksum >> 8) & 0xff;
    TX_buffer[TX_write_index][21] = deviceChecksum;

    TX_buffer[TX_write_index][22] = (deviceModChecksum >> 8) & 0xff;
    TX_buffer[TX_write_index][23] = deviceModChecksum;

    TX_buffer[TX_write_index][24] = (deviceFadeChecksum >> 8) & 0xff;
    TX_buffer[TX_write_index][25] = deviceFadeChecksum;

    TX_buffer[TX_write_index][26] = statePostscaler;
    TX_buffer[TX_write_index][27] = txPower;
    TX_buffer[TX_write_index][28] = txLowPower;

    TX_buffer[TX_write_index][29] = (deviceOnActionChecksum >> 8) & 0xff;
    TX_buffer[TX_write_index][30] = deviceOnActionChecksum;

    TX_buffer[TX_write_index][31] = relayRisingDelay;
    TX_buffer[TX_write_index][32] = relayRisingLevel;

    TX_buffer[TX_write_index][33] = minSetpoint;

    TX_buffer[TX_write_index][34] = (connectorSceneChecksum >> 8) & 0xff;
    TX_buffer[TX_write_index][35] = connectorSceneChecksum;

    TX_buffer[TX_write_index][36] = (deviceSceneSwitchChecksum >> 8) & 0xff;
    TX_buffer[TX_write_index][37] = deviceSceneSwitchChecksum;

    BYTE wiBit = 1 << (TX_write_index & 7);
    TX_low_power[TX_write_index / 8] &= ~wiBit;

    setTxBuffReady();
}

static CHAR
actionState( int deviceid )
{
  BYTE action = associatedDevices[deviceid].on_action;

  if( action <= 100 )
    return action;
  switch(action - 101) {
  case 0:
      return occSetpoint;
  case 1:
      return scene[0];
  case 2:
      return scene[1];
  case 3:
      return scene[2];
  case 4:
      return scene[3];
  case 5:
      return scene[4];
  case 6:
      return scene[5];
  case 7:
      return scene[6];
  case 8:
      return scene[7];
  case 9:
      return scene[8];
  case 10:
      return scene[9];
  case 11:
      return scene[10];
  case 12:
      return scene[11];
  case 13:
      return scene[12];
  case 14:
      return scene[13];
  case 15:
      return scene[14];
  case 16:
      return scene[15];
  }
  return -1;
}

void CheckForReceivedDataPacket()
{
    BYTE repeatOffset;
    BYTE i;
    bool hasFooter;
    BYTE frameLength;

    countError(RX_COUNT);

    RX_packet[0] = TRX_GetLength();  // get rx len from radio and put in RX_packet[0]
    for (i = 0; i < RX_packet[0]; i++)		// get the current buffer data
        RX_packet[i + 1] = TRX_GetData(i);

    packetRssi = TRX_GetRSSI();                 // get rssi incase we repeat later
    TRX_ReleasePacket();                        // release packet

    radioResetTimer = 0;

    frameLength = RX_packet[0];			// frame length

    recNodeID = ((DWORD)RX_packet[1] << 24) |   // Node ID identifier
                ((DWORD)RX_packet[2] << 16) |
                ((DWORD)RX_packet[3] << 8) |
                 (DWORD)RX_packet[4];

    recPacketCounter = ((DWORD)RX_packet[5] << 16) |    // packet counter
                       ((DWORD)RX_packet[6] << 8) |
                        (DWORD)RX_packet[7];

    command = ((WORD)RX_packet[9] << 8) + RX_packet[10];    // command to perform
    hasFooter = HasFooter(command, frameLength, RX_packet, &repeatOffset);
    if( hasFooter )
        rxedGatewayID = RepeaterGateway(repeatOffset, RX_packet);

    if(frameLength < SIZE_CONNECTOR_HEADER_BYTES)
    {
        countError(ERR_REQ_SIZE_TOO_SMALL);
        return;
    }

    /* must match gateway or be an UPDATE_CONNECTOR for us */
    if( !(((command == UPDATE_CONNECTOR || command == IS_GATEWAY_REQUEST) &&
           recNodeID == myNodeID) ||
          !hasFooter ||
	  is_unowned() ||
	  is_local_vacancy_connector() ||
	  rxedGatewayID == gatewayID))
    {
        countError(RX_GW_ID_MISMATCH);
        return;
    }

    // validate length of request
    if (frameLength != repeatOffset - 1 + (hasFooter ? SIZE_RADIO_TRAILER_BYTES : 0))  // 10 bytes in repeat trailer added by sending radio
    {
        countError(ERR_REQ_SIZE_INVALID);
        return;
    }

    repeat = ShouldRepeatPacket(recNodeID, hasFooter, rxedGatewayID);

    //*********************************************************************
    // Check packet type
    //*********************************************************************

    // packet sent directly to this connector or the connector is not
    // owned by a gateway or is operating in local vacancy mode
    if (recNodeID == myNodeID &&
	(command == UPDATE_CONNECTOR ||
	 command == IS_GATEWAY_REQUEST ||
	 (hasFooter && rxedGatewayID == gatewayID) ||
	 is_unowned() ||
	 is_local_vacancy_gw_id(gatewayID)))
    {
        if (RadioUtils_IsValidRxPacketCount(myRecPacketCounter, recPacketCounter) &&
            RadioUtils_IsNotNearTxPacketCount(myPacketCounter, recPacketCounter))
        {                                       // check for valid packet
            myRecPacketCounter = recPacketCounter;

            if (command == REQUEST_STATE)       // report back current state
            {
                AddPollResponseToTXBuffer(5);
                if (!connectorHasBeenPolled)
                    connectorHasBeenPolled = true;
                gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;
            }
            else if (command == IS_GATEWAY_REQUEST)
            {
                txRespPrep(5);

                loadRespHeaderIntoTxBuff(IS_GATEWAY, SIZE_IS_GATEWAY + sizeof(rxedGatewayID));

                TX_buffer[TX_write_index][8] = rxedGatewayID != 0 && rxedGatewayID == gatewayID;
                TX_buffer[TX_write_index][11] = rxedGatewayID >> 24;
                TX_buffer[TX_write_index][12] = rxedGatewayID >> 16;
                TX_buffer[TX_write_index][13] = rxedGatewayID >> 8;
                TX_buffer[TX_write_index][14] = rxedGatewayID;

                BYTE wiBit = 1 << (TX_write_index & 7);
                TX_low_power[TX_write_index / 8] &= ~wiBit;

                setTxBuffReady();
            }
            else if (command == ADD_DEVICE && is_legal_gw_id(gatewayID))	    // associate with new device
            {
                DWORD checkNodeID = 0;
                checkNodeID = ((DWORD)RX_packet[11] << 24) +    // get device ID
                              ((DWORD)RX_packet[12] << 16) +
                              ((DWORD)RX_packet[13] << 8) +
                               (DWORD)RX_packet[14];

                for (i = 0; i < deviceCount; i++)   // check if already exists
                {
                    if (checkNodeID == associatedDevices[i].id ||
                        associatedDevices[i].id > checkNodeID)
                        break;
                }

                if ((i == deviceCount || associatedDevices[i].id > checkNodeID) &&
                    deviceCount < MAX_DEVICES) // new device
                {
                    CHAR j; // signed due to terminating < 0 and MAX_DEVICES limit
                    BYTE k;
                    for( j = deviceCount - 1; deviceCount > 0 && j >= i; j-- )
                        associatedDevices[j + 1] = associatedDevices[j];

                    associatedDevices[i].id = checkNodeID;
                    associatedDevices[i].packetCounter = 0;
                    associatedDevices[i].mod = 0;
                    associatedDevices[i].fade = 0;
                    associatedDevices[i].on_action = 100;

                    for(k = 0;k< MAX_BUTTON;++k) {
                        associatedDevices[i].button_scene[k] = (k + 1) & 0xf;
                    }

                    deviceCount++;
                    update_connector_checksums();
                }
            }
            else if (command == UPDATE_DEVICE_MOD && is_legal_gw_id(gatewayID))
            {
                DWORD checkNodeID = 0;
                checkNodeID = ((DWORD)RX_packet[11] << 24) +
                              ((DWORD)RX_packet[12] << 16) +
                              ((DWORD)RX_packet[13] << 8) +
                               (DWORD)RX_packet[14];

                for (i = 0; i < deviceCount; i++) {
                    if (checkNodeID == associatedDevices[i].id) {
                        associatedDevices[i].mod = RX_packet[15];
                        deviceModChecksum = AssociatedDevicesModChecksum();
                        updEEdevices = true;
                        break;
                    }
                }
            }
            else if (command == UPDATE_DEVICE_FADE && is_legal_gw_id(gatewayID))     // associate with new device
            {
                DWORD checkNodeID = 0;
                checkNodeID = ((DWORD)RX_packet[11] << 24) +
                              ((DWORD)RX_packet[12] << 16) +
                              ((DWORD)RX_packet[13] << 8) +
                               (DWORD)RX_packet[14];

                for (i = 0; i < deviceCount; i++) {
                    if (checkNodeID == associatedDevices[i].id) {
                        associatedDevices[i].fade = RX_packet[15];
                        deviceFadeChecksum = AssociatedDevicesFadeChecksum();
                        updEEdevices = true;
                        break;
                    }
                }
            }
            else if (command == UPDATE_DEVICE_ON_ACTION && is_legal_gw_id(gatewayID))     // associate with new device
            {
                DWORD checkNodeID = 0;
                checkNodeID = ((DWORD)RX_packet[11] << 24) +
                              ((DWORD)RX_packet[12] << 16) +
                              ((DWORD)RX_packet[13] << 8) +
                               (DWORD)RX_packet[14];

                for (i = 0; i < deviceCount; i++) {
                    if (checkNodeID == associatedDevices[i].id) {
                        associatedDevices[i].on_action = RX_packet[15];
                        deviceOnActionChecksum = AssociatedDevicesOnActionChecksum();
                        updEEdevices = true;
                        break;
                    }
                }
            }
            else if ( command == UPDATE_DEVICE_SCENE_SELECT && is_legal_gw_id(gatewayID))
            {
                DWORD checkNodeID = 0;
                checkNodeID = ((DWORD)RX_packet[11] << 24) +
                              ((DWORD)RX_packet[12] << 16) +
                              ((DWORD)RX_packet[13] << 8) +
                               (DWORD)RX_packet[14];

                for (i = 0; i < deviceCount; i++) {
                    if (checkNodeID == associatedDevices[i].id) {
                        associatedDevices[i].button_scene[0] = RX_packet[15];
                        associatedDevices[i].button_scene[1] = RX_packet[16];
                        associatedDevices[i].button_scene[2] = RX_packet[17];
                        associatedDevices[i].button_scene[3] = RX_packet[18];
                        deviceSceneSwitchChecksum = AssociatedDevicesSceneSelectChecksum();
                        break;
                    }
                }
            }
            else if (command == REMOVE_DEVICE && gatewayID != 0)  // disassociate with a switch
            {
                DWORD checkNodeID = 0;
                checkNodeID = ((DWORD)RX_packet[11] << 24) +    // get device ID
                              ((DWORD)RX_packet[12] << 16) +
                              ((DWORD)RX_packet[13] << 8) +
                               (DWORD)RX_packet[14];

                for (i = 0; i < deviceCount && checkNodeID != associatedDevices[i].id; i++);
                if (i != deviceCount) {
                    while( i++ < deviceCount - 1 )
                      associatedDevices[i - 1] = associatedDevices[i];

                    --deviceCount;
                    update_connector_checksums();
                }
            }
            else if (command == REMOVE_ALL_DEVICES && gatewayID != 0) // remove all associations
            {
                deviceCount = 0;
                update_connector_checksums();
            }
            else if (command == RESET_CONNECTOR && gatewayID != 0) // remove all associations
            {
                channel = 1;
                repeater = false;
                occSetpoint = 50;
                occupiedState = false;
                gatewayID = 0;
                updEEstatus = true;
                flashLight = false;  // used only if isFlashLightConn()
                maxSetpoint = 100;   // used only if isDimmingConn()
                minSetpoint = 0;
                vacancyTimer = 0;    // used only if isLocalVacancyConn() - restart count-up timer
                deviceCount = 0;
                update_connector_checksums();
                txPower = -6;
                txLowPower = -20;
                relayRisingDelay = 0;
                relayRisingLevel = 0;
            }
            else if (command == UPDATE_CONNECTOR)   // update connector
            {
                //* If this UPDATE_CONNECTOR packet didn't come from
                //* owning gateway, then do not accept any state
                //* changes.  Note that if the gatewayID is 0, or if
                //* the connector is operating in local_vacancy mode,
                //* then this connector is unowned.

                DWORD target_gw_id = ((DWORD) RX_packet[26] << 24) +
                                     ((DWORD) RX_packet[27] << 16) +
                                     ((DWORD) RX_packet[28] << 8) +
                                      (DWORD) RX_packet[29];

		if ((gatewayID != 0 ) &&
		    (target_gw_id != gatewayID) &&
		    (!is_local_vacancy_gw_id(gatewayID))) {
                    goto packet_processing_done;
                }

                // If we are attempting to set the gateway to be the
                // local vacancy gateway id and we are not a local
                // vacancy connector, reject the packet.

                if ( is_local_vacancy_gw_id(target_gw_id) &&
                     !isLocalVacancyConn(MY_DEV_TYPE)) {
                    goto packet_processing_done;
                }

                // defaultSetpoint is the unoccupied state
                defaultSetpoint = RX_packet[8] & 0x7F;  // don't include 0x80 bit if set
		// 0x80 = invert dim, 0x00 = normal dim
                invertDimIfNewStateZero = ((RX_packet[8] >> 7) & 1) == 1;
                invertDimChanged = true;

                repeater = ((RX_packet[11] >> 2) & 1) == 1;
                channel = RX_packet[11] & 0x03;

                if (RX_packet[12] != 0xFF) {
                    if (isFlashLightConn(MY_DEV_TYPE) && flashLight)
                        relayOutputLevelSave = RX_packet[12] * 6;
                    else
                        newOutputLevel = RX_packet[12] * 6;
                }

                if (RX_packet[14] & 0x80)                   // upper 1 bit
                    occupiedState = true;
                else
                    occupiedState = false;
                occSetpoint = RX_packet[14] & 0x7F;         // lower 7 bits

                if ( is_local_vacancy_connector())
                {
                    if (RX_packet[15] & 0x80)                   // check for setting max seconds
                        vacancyMaxSeconds = (WORD)(RX_packet[15] & 0x7F) * 5 * 60;
                    else
                        maxSetpoint = RX_packet[15] & 0x7f;
                } else
                    maxSetpoint = RX_packet[15] & 0x7f;

                scene[0] = RX_packet[16];
                scene[1] = RX_packet[17];
                scene[2] = RX_packet[18];
                scene[3] = RX_packet[19];
                if (statePostscaler != RX_packet[20])
                    activeOutputLevel = !newOutputLevel; /* ensure output update */
                statePostscaler = RX_packet[20];

                // TODO - Need to add legality check so we don't put the connector
                // into an illegal radio power level for the radio region.
                CHAR newTxPower = RX_packet[21];
                if (txPower != newTxPower)
                    TRX_PowerLevel(TRX_dBmToPower(newTxPower));
                txPower = newTxPower;

                txLowPower = RX_packet[22];
                relayRisingDelay = RX_packet[23];
                relayRisingLevel = RX_packet[24];
                minSetpoint = RX_packet[25];
                gatewayID = ((DWORD)RX_packet[26] << 24) +
                            ((DWORD)RX_packet[27] << 16) +
                            ((DWORD)RX_packet[28] << 8) +
                             (DWORD)RX_packet[29];

                if (maxSetpoint * 6 < newOutputLevel)
                    newOutputLevel = maxSetpoint * 6;
                if (maxSetpoint * 6 < relayOutputLevelSave)
                    relayOutputLevelSave = maxSetpoint * 6;
                if (newOutputLevel != 0 && minSetpoint * 6 > newOutputLevel)
                    newOutputLevel = minSetpoint * 6;
                if (relayOutputLevelSave != 0 && minSetpoint * 6 < relayOutputLevelSave)
                    relayOutputLevelSave = minSetpoint * 6;
                fadeToState = newOutputLevel / 6;

                gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;

                updEEstatus = true;
            }
            else if (command == UPDATE_CONNECTOR_SCENE )
            {
                //* If this UPDATE_CONNECTOR_SCENE packet didn't come
                //* from owning gateway, then do not accept any state
                //* changes.  Note that if the gatewayID is 0, or if
                //* the connector is operating in local_vacancy mode,
                //* then this connector is unowned.


                DWORD target_gw_id = ((DWORD) RX_packet[27] << 24) +
                                     ((DWORD) RX_packet[28] << 16) +
                                     ((DWORD) RX_packet[29] << 8) +
                                      (DWORD) RX_packet[30];

                if ((gatewayID != 0 ) && (target_gw_id != gatewayID) &&
                    (!is_local_vacancy_gw_id(gatewayID))) {
                        goto packet_processing_done;
                }

                scene[0] = RX_packet[11];
                scene[1] = RX_packet[12];
                scene[2] = RX_packet[13];
                scene[3] = RX_packet[14];
                scene[4] = RX_packet[15];
                scene[5] = RX_packet[16];
                scene[6] = RX_packet[17];
                scene[7] = RX_packet[18];
                scene[8] = RX_packet[19];
                scene[9] = RX_packet[20];
                scene[10] = RX_packet[21];
                scene[11] = RX_packet[22];
                scene[12] = RX_packet[23];
                scene[13] = RX_packet[24];
                scene[14] = RX_packet[25];
                scene[15] = RX_packet[26];
                connectorSceneChecksum = ConnectorSceneChecksum();

                gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;
            }
            else if (command == REQ_FW_INFO)
            {
                reqFwInfo_handler();
            }
            else if (command == REQ_FW_MODE && gatewayID != 0)
            {
                reqFwMode_handler();

                gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;
            }
            else if (command == REQ_FW_MISSING && gatewayID != 0)
            {
                reqFwMissing_handler();

                gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;
            }
            else if (command == REQ_DIAG_OP)
            {
                reqDiag_handler();

                gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;
            }
        }
    }
    // Else note my Node ID (not point-to-point), so check for Multicast address (upper byte matches my Node ID (type device) and lower three bytes all one bits)
    else if (recNodeID == ((myNodeID & 0xFF000000) | 0x00FFFFFF) && hasFooter && gatewayID != 0 && rxedGatewayID == gatewayID)
    {
	// We received a message that the previous conditional
	// verified was from our owning gateway.  Therefore, reset the
	// gateway disassociate timeout.
	gatewayDisassociateTimeout = GATEWAY_DISASSOCIATE_TIMEOUT;
        if (command == REQ_FW_UPGRADE)
        {
            reqFwUpgrade_handler();
            fadeToState = 100;
            newOutputLevel = fadeToState * 6;
        }
        // NOTE: the next two requests share the same command code:
        // (1) REQUEST FW DATA HEADER - bit 6 of Page Id field 0 and length 27 (or 28 if repeated)  - contains MD5 Sum for Page of FW (16 blocks per page)
        // (2) REQUEST FW DATA        - bit 6 of Page Id field 1 and length 124 (or 125 if repeated) - contains MD5 Sum for next block of data in Page
        else if ((command == REQ_FW_DATA_HEADER_OR_DATA) && ((RX_packet[11] & 0x40) == 0x00))  // Page ID bit 6 (0x40 bit) should be a zero else a FW_DATA req
        {
            reqFwDataHeader_handler();
        }
        else if (command == REQ_FW_DATA_HEADER_OR_DATA)
        {
            if( !reqFwData_handler() )
                repeat = false;
        }
        else if (command == REQ_FW_APPLY)
        {
            reqFwApply_handler(((txPower + 26) << 3) |
                               ((repeater ? 1 : 0) << 2) |
                               (channel & 0x03), gatewayID);
        }
	else if (command == UPDATE_CONNECTOR_POSTSCALER )
        {

	    /* ensure output update */
	    activeOutputLevel = !newOutputLevel;
	    statePostscaler = RX_packet[11];
  	    updEEstatus = true;
        }
    }
    // else not my Node ID (not point-to-point) or multicast, check if request from an associated device (e.g., sensor, switch, ...)
    else if( fwUpgradeTimer == 0 && (rxedGatewayID == gatewayID || !hasFooter) )
    {
        bool spoof = RX_packet[8] == 0xFF;      // check for packet spoof, normally battery voltage location

        /* If not doing f/w update, find matching association */
        for (i = 0; i < deviceCount && recNodeID != associatedDevices[i].id; i++);

        doCommand = i != deviceCount;
        if (!doCommand)
            countError(RX_FROM_DEVICE_NOT_ASSOCIATED);

        // check for valid packet
        if(doCommand && spoof && RadioUtils_IsValidRxPacketCount(spoofPktCntr, recPacketCounter))
        {
            spoofPktCntr = recPacketCounter;
        }
        else if(doCommand && !spoof && RadioUtils_IsValidRxPacketCount(associatedDevices[i].packetCounter, recPacketCounter))
        {
            associatedDevices[i].packetCounter = recPacketCounter;
        }
        else
            doCommand = false;

        if (doCommand)                  // passed all checks, process command
        {
            SHORT assignState = -1;
	        SHORT scene_index = 0;
            flashLight = false;

            switch(command) {
            case 0:
                // heartbeat from Switch or Motion Sensor devices
                break;
            case TURN_OFF:
              /* turn off */
              assignState = 0;
              occupiedState = true;
              break;
            case TURN_ON:
	      assignState = actionState(i);
              occupiedState = true;
              break;
            case INCREASE_DIM:
              /* increase dim by 10 */
              assignState = fadeToState < 90 ? fadeToState + 10 : 100;  // done even if !isDimmingConn() because UpdateOutputState() ignores
              occupiedState = true;
              break;
            case DECREASE_DIM:
              /* decrease dim by 10 */
              assignState = fadeToState < 10 ? fadeToState : fadeToState > 20 ? fadeToState - 10 : 10;  // done even if !isDimmingConn() because UpdateOutputState() ignores
              occupiedState = true;
              break;
            case SET_LIGHT_LEVEL:
		// Gateway's "Set Light Level"
		// (GW Spoofs NID of Sensor and Switches)
		/* set level */
              assignState = RX_packet[11];
              occupiedState = true;
              break;
	    case SET_SCENE:
		// Gateway Spoof of SET_SCENE command
		// Force the connector to a particular scene value
		scene_index = RX_packet[11] - 1;
		if ( scene_index >= 0 && scene_index < 16 ) {
		    assignState = scene[scene_index];
		    occupiedState = true;
		}
		break;
            case 60:  // Gateway's "Flash Light" (GW Spoofs NID of Sensor and Switches)
                if (!isSparrowOutletConn(MY_DEV_TYPE))
                    flashLight = true; /* set flag to flash light */
                break;
            case 1:  // Motion/Occ Sensor Device's "Motion Last 4 Minutes")
            case 65:  // Motion/Occ Sensor Device's "Turn Lights On"
              if( !occupiedState )
                  assignState = occSetpoint; /* occupancy sensor turn on */
              if(isLocalVacancyConn(MY_DEV_TYPE))
                  vacancyTimer = 0;
              occupiedState = true;
              break;
            case SCENE_1:  // (GW May spoof Scene commands)
              scene_index = (associatedDevices[i].button_scene[0]) - 1;
              if( scene_index >= 0 && scene_index < 16 ) {
                assignState = scene[scene_index];
                occupiedState = true;
              }
              break;
            case SCENE_2:  // (GW May spoof Scene commands)
	          scene_index = (associatedDevices[i].button_scene[1]) - 1;
              if( scene_index >= 0 && scene_index < 16 ) {
                assignState = scene[scene_index];
                occupiedState = true;
              }
              break;
            case SCENE_3:    // (GW May spoof Scene commands)
	          scene_index = (associatedDevices[i].button_scene[2]) - 1;
              if( scene_index >= 0 && scene_index < 16 ) {
                assignState = scene[scene_index];
                occupiedState = true;
              }
              break;
            case SCENE_4:  // (GW May spoof Scene commands)
	          scene_index = (associatedDevices[i].button_scene[3]) - 1;
              if( scene_index >= 0 && scene_index < 16 ) {
                assignState = scene[scene_index];
                occupiedState = true;
              }
              break;
            case 70:
		// Gateway's "Occupancy On" (GW Spoofs NID of Sensor and Switches)
		occupiedState = true;
		break;
            case 75:
		// Gateway's "Occupancy Off" (GW Spoofs NID)
		occupiedState = false;
		break;
            case 80:  // gw indicates room timed out
              /* set level to Default setpoint and clear occupied flag */
              /* default state ignores fade */
              fadeToState = defaultSetpoint;  // unoccupied state
              newOutputLevel = fadeToState * 6;
              occupiedState = false;
              break;
            case 81:  // possible GW request
              /* set level to Default setpoint */
              assignState = defaultSetpoint;  // unoccupied state
              break;
            }

            if( assignState != -1 ) {
                if( assignState > maxSetpoint )
                    assignState = maxSetpoint;
                if( assignState != 0 && assignState < minSetpoint )
                    assignState = minSetpoint;
                fadeToState = assignState;
                if( isDimmingConn(MY_DEV_TYPE) &&
		    assignState * 6 != relayOutputLevel &&
		    associatedDevices[i].fade != 0 ) {
                  BYTE fade = associatedDevices[i].fade;
                  WORD fadeSeconds = fade & FADE_VALUE;
                  if( fade & FADE_MINUTES )
                    fadeSeconds *= 60;
                  fadeAfterTime = abs(fadeSeconds * 600L / (LONG) (assignState * 6L - relayOutputLevel));
                  fadeTickCount = 0;
                  fadeLastTime = TMR4;
                } else
                  newOutputLevel = assignState * 6;
            }
        }
    }

packet_processing_done:

    // check if recently seen packet
    for (i = 0; i < WINDOW_SIZE; i++)
    {
        if (recNodeID == lastNodeID[i] &&
            recPacketCounter == lastPacketCounter[i])
        {
            if (repeater && repeat)
                countError(RX_RECENT_PKT_NO_REPEAT);
            repeat = 0;
            break;
        }
    }

    if (i < WINDOW_SIZE)
    {
        lastNodeID[i] = recNodeID;
        lastPacketCounter[i] = recPacketCounter;
    }
    else
    {
        lastNodeID[lastPacketUpdate] = recNodeID;
        lastPacketCounter[lastPacketUpdate++] = recPacketCounter;
    }

    if (lastPacketUpdate >= WINDOW_SIZE)
        lastPacketUpdate = 0;

    //*********************************************************************
    // Add packet to TX buffer if repeater
    //*********************************************************************
    AddPacketToTxBufferIfRepeater(command, frameLength);
}

void AddPacketToTxBufferIfRepeater( WORD command, WORD frameLength )
{
    if( !repeat )
        return;

    BYTE repeatOffset = RepeatOffset(RX_packet[1], command, RX_packet);

    if (frameLength == repeatOffset - 1)
    { // first repeat
        TX_ttl[TX_write_index] = repeater ? 6 : 1; /* max ttl for tx hop count*/
        TX_rssi[TX_write_index] = packetRssi;
    }
    else if( RX_packet[repeatOffset + 8] == 1 )
    {
        countError(RX_NO_REPEAT_HOP_COUNT_ZERO);
        return;
    }
    else {  // from gateway, or already repeated
        TX_ttl[TX_write_index] = RX_packet[repeatOffset + 8] - 1;  // decrement hop count used when tx
        TX_rssi[TX_write_index] = /* update rssi with lower value */
          packetRssi < RX_packet[repeatOffset + 9] ?
          packetRssi : RX_packet[repeatOffset + 9];
    }

    BYTE wiBit = 1 << (TX_write_index & 7);
    TX_low_power[TX_write_index / 8] &= ~wiBit;
    if( !repeater )
      TX_low_power[TX_write_index / 8] |= wiBit;

    WORD repeatSize = repeatOffset - 1; /* TX does job of supplying footer */

    int i = 0;
    srand(TMR1);			// randomize delay before sending packet
    rand_int = 10 + (rand() % 40);	// min is 10ms, max is 50ms

    TX_delay[TX_write_index] = rand_int;
    TX_buffer[TX_write_index][0] = repeatSize;
    for (i = 1; i <= repeatSize; i++)
    {					// add RX data to TX frame
        TX_buffer[TX_write_index][i] = RX_packet[i];
    }

    TX_write_index++;
    if( TX_write_index == MAX_TX_BUFFERS )
      TX_write_index = 0;

    if( TX_read_index == TX_write_index ) {
      TX_read_index++;
      if( TX_read_index == MAX_TX_BUFFERS )
        TX_read_index = 0;
      countError(ERR_RX_DROP_OLD_RX_BUF_FOR_REPEAT);
    }
}


void CheckTRXErrorStatus()
{
        errorStatus = TRX_CheckError();
        if (errorStatus)
        {
            countRadioErr(errorStatus);

#if defined (TX_UNSOLICITED_ERROR_MESSAGE_TO_GW)
            // send an immediate packet
            TRX_ClearFrame();			// clear TX frame

            myPacketCounter = MODU24_ADD(myPacketCounter, 1u);

            TRX_WriteData(myNodeID >> 24);
            TRX_WriteData(myNodeID >> 16);
            TRX_WriteData(myNodeID >> 8);
            TRX_WriteData(myNodeID);
            TRX_WriteData(myPacketCounter >> 16);
            TRX_WriteData(myPacketCounter >> 8);
            TRX_WriteData(myPacketCounter);
            TRX_WriteData(0x00);
            TRX_WriteData(0x01);
            TRX_WriteData(0x94);        // error command, 404
            TRX_WriteData(errorStatus);

            if (!TRX_TxPacket())        // try to send frame data
            {
                radioResetTimer = radioResetLimit;  // reset radio next loop
            }
#endif
        }

}

void CheckTheTxBufferForUnsentPackets()
{
    // data in buffer to transmit
    if (TX_delay_counter < TX_delay[TX_read_index])
    {
        TX_delay_counter++;			// increment counter
        DelayMs(1);
    }
    else
    {						// TX packet
        int i = 0;
        bool low_power = TX_low_power[TX_read_index / 8] & (1 << (TX_read_index & 7));
        BYTE length = TX_buffer[TX_read_index][0];
        TRX_ClearFrame();			// clear TX frame
        for (i = 1; i <= length; i++)
            TRX_WriteData(TX_buffer[TX_read_index][i]);
        WORD command = ((WORD) TX_buffer[TX_read_index][9] << 8) |
            (WORD) TX_buffer[TX_read_index][10];
        if (command != IS_GATEWAY) {
            // Add trailer to Radio's Buffer
            TRX_WriteData(gatewayID >> 24);    // add gateway ID
            TRX_WriteData(gatewayID >> 16);
            TRX_WriteData(gatewayID >> 8);
            TRX_WriteData(gatewayID);
        }

        TRX_WriteData(myNodeID >> 24);     // add my ID
        TRX_WriteData(myNodeID >> 16);
        TRX_WriteData(myNodeID >> 8);
        TRX_WriteData(myNodeID);

        TRX_WriteData(TX_ttl[TX_read_index]);
        TRX_WriteData(TX_rssi[TX_read_index]);

        if (low_power)
            TRX_PowerLevel(TRX_dBmToPower(txLowPower));

        if (!TRX_TxPacket())			// try to send frame data
            radioResetTimer = radioResetLimit;  // reset radio next loop
        else {
            if (low_power)
                TRX_PowerLevel(TRX_dBmToPower(txPower));
            // packet successfully sent
            TX_read_index++;
            if (TX_read_index == MAX_TX_BUFFERS)
                TX_read_index = 0;
        }
        TX_delay_counter = 0;
    }
}

void DoFlashCommand()
{
    if (activeOutputLevel != 0)
    {
        if (!flashLightStart)
        {
            TMR1 = 0;
            flashLightTimer = 0;
            flashLightStart = true;
            relayOutputLevelSave = activeOutputLevel;        // save previous state
            newOutputLevel = 10 * 6;
        }
        else if (flashLightTimer)
        {
            flashLightTimer = 0;
            if (!flashLightDone)
            {
                newOutputLevel = 100 * 6;
                flashLightDone = true;
            }
            else
            {
                flashLight = false;
                flashLightStart = false;
                flashLightDone = false;
                newOutputLevel = relayOutputLevelSave;      // revert to previous state
            }
        }
    }
    else
    {
        flashLight = false;
        flashLightStart = false;
        flashLightDone = false;
    }
}

void UpdateOutputState()

{
    if (newOutputLevel > maxSetpoint * 6)         // make sure we don't exceed our max value
        newOutputLevel = maxSetpoint * 6;
    WORD outputLevel = newOutputLevel;
    if (outputLevel != 0 && outputLevel < minSetpoint * 6)
        outputLevel = minSetpoint * 6;

    if (isBiPinRelayConn(MY_DEV_TYPE)) {
        // Setup timer to fire an interrupt when we should drop the pin
        TMR5= 0x0000;

        if ( outputLevel < 1 && isLineDimmingConn(MY_DEV_TYPE) && invertDimIfNewStateZero) {
            // Line dimming connector with inversion logic enabled.
            newOutputLevel = 0;
            energize_bipin_on_coil(MY_DEV_TYPE);
            relayRisingCountdown = 0;
            T5CONbits.TON = 1;
        } else if (outputLevel < 1) {
            // Turn off relay
            newOutputLevel = 0;
            energize_bipin_off_coil(MY_DEV_TYPE);
            relayRisingCountdown = 0;
            T5CONbits.TON = 1;  // Turn on the timer to drop relay GPIO
        } else if (activeOutputLevel == 0 && outputLevel > 0) {
            // Turn on relay
            energize_bipin_on_coil(MY_DEV_TYPE);
            if(isLocalVacancyConn(MY_DEV_TYPE))
                occupiedState = true;
            relayRisingCountdown = relayRisingDelay;
            T5CONbits.TON = 1;  // Turn on the timer to drop relay GPIO
        }
    } else {
        if (outputLevel == 0) {
            MOMENTARY_RELAY_ON = 0;                               // Turn off relay
            relayRisingCountdown = 0;
        }
        else if (activeOutputLevel == 0 && outputLevel > 0) {
            MOMENTARY_RELAY_ON = 1;                               // Turn on relay
            if(isLocalVacancyConn(MY_DEV_TYPE))
                occupiedState = true;
            relayRisingCountdown = relayRisingDelay;
        }
    }

    // We have adjusted the relays, so the flag that says that the
    // inversion setting has changed can be cleared.
    invertDimChanged = false;

    relayOutputLevel = outputLevel;
    if( relayRisingCountdown == 0 )
        activeOutputLevel = relayOutputLevel;  // update current state
    else {
        WORD relayRisingOutputLevel = relayRisingLevel * 6;
        if( relayRisingOutputLevel > outputLevel )
            relayRisingOutputLevel = outputLevel;
        if( activeOutputLevel == relayRisingOutputLevel )
            return;
        activeOutputLevel = relayRisingOutputLevel;
    }

    if (isDimmingConn(MY_DEV_TYPE))
    {
        WORD outputDimLevel = activeOutputLevel * statePostscaler / 100;
        // Update Dim level
        if (isLineDimmingConn(MY_DEV_TYPE)) {
            if (outputDimLevel != 0) {
                OC1RS = 600 - outputDimLevel;
            } else if (invertDimIfNewStateZero) {
                OC1RS = 0;
            } else {
                OC1RS = 600;
            }
        } else if (isEmergeConn(MY_DEV_TYPE)) {
            if (isEmergeSlowerPwmConn(MY_DEV_TYPE)) {
                // update dim level
                OC1RS = (DWORD) outputDimLevel * PWM_VALUE / 600;
            } else {
                OC1RS = 600 - outputDimLevel;     // update dim level
            }
        } else {
            // Set Dim Levels
            if (outputDimLevel != 0) {
                OC1RS = 600 - outputDimLevel;
            } else if (invertDimIfNewStateZero) {
                // Inverted (for now on Smart Connector Only, not Emerge)
                OC1RS = 600;
            } else {
                // Normal (set to 10V), use counter to delay min 1s
                offDimLevelCounter = 0;
            }
        }
    }
}

void LoadNVRamData()
{
    int i = 0;
    WORD channelAndStatePostscaler, deviceCountAndTxPower;

    channelAndStatePostscaler = DataEERead(CHANNEL_AND_STATE_POSTSCALER_ADDR);		Nop();
    channel = channelAndStatePostscaler;
    // check for initialized EEPROM
    if (channel > 3)
    {
        WORD chn_rpt_tx_power = flash_boot_chn_rpt_tx_power();
        if( chn_rpt_tx_power != 0xffff && (chn_rpt_tx_power & 3) != 0 &&
            (chn_rpt_tx_power & 3) != 3 ) {
            channel = chn_rpt_tx_power & 3;
            repeater = ((chn_rpt_tx_power >> 2) & 1) == 1;
            if (((chn_rpt_tx_power >> 3) & 0x1f) != 0)
                txPower = ((chn_rpt_tx_power >> 3) & 0x1f) - 26;
            gatewayID = flash_boot_gateway();
            if( gatewayID == 0xffffffff )
                gatewayID = 0;
        } else
            channel = 1;
        DataEEWrite(channel | (((WORD) statePostscaler) << 8), CHANNEL_AND_STATE_POSTSCALER_ADDR);
        DataEEWrite(repeater ? 1 : 0, REPEATER_ADDR);
        DataEEWrite((gatewayID >> 16), GATEWAYID_ADDR);
        DataEEWrite(gatewayID, GATEWAYID_ADDR + 1);
        DataEEWrite(deviceCount | (((WORD) (txPower + 25)) << 8), DEVICECOUNT_AND_TX_POWER_ADDR);
        DataEEWrite(defaultSetpoint | (invertDimIfNewStateZero ? 0x80 : 0), DEFAULT_SET_POINT_ADDR);  // if using invert Dim 0x80 bit set
    }
    else
    {
        statePostscaler = channelAndStatePostscaler >> 8;
        repeater = DataEERead(REPEATER_ADDR) == 1;	Nop();
        readEEdata1 = DataEERead(GATEWAYID_ADDR);   	Nop();
        readEEdata2 = DataEERead(GATEWAYID_ADDR + 1);  	Nop();
        gatewayID = ((DWORD)readEEdata1 << 16) + readEEdata2;
        deviceCountAndTxPower = DataEERead(DEVICECOUNT_AND_TX_POWER_ADDR);	Nop();
        deviceCount = deviceCountAndTxPower;
        txPower = ((char) (deviceCountAndTxPower >> 8)) - 25;
        defaultSetpoint = DataEERead(DEFAULT_SET_POINT_ADDR); Nop();
        invertDimIfNewStateZero = ((defaultSetpoint >> 7) & 1) == 1;  // 0x80 = invert dim, 0 = normal dim
        defaultSetpoint &= 0x7F;  // make sure 0x80 bit clear


        for (i = 0; i < deviceCount; i++)
        {
            readEEdata1 = DataEERead(ASSOCIATEDDEVICES_ADDR + (i*2));       Nop();
            readEEdata2 = DataEERead(ASSOCIATEDDEVICES_ADDR + (i*2) + 1);   Nop();
            associatedDevices[i].id = ((DWORD)readEEdata1 << 16) + readEEdata2;

            associatedDevices[i].packetCounter = 0;
        }

        for (i = 0; i < deviceCount; i++)
        {
            WORD modAndFade = DataEERead(ASSOCIATEDDEVICES_ADDR + (MAX_DEVICES * 2) + i * 2);       Nop();
            associatedDevices[i].mod = modAndFade;
            associatedDevices[i].fade = modAndFade >> 8;
            BYTE on_action = DataEERead(ASSOCIATEDDEVICES_ADDR + (MAX_DEVICES * 2) + i * 2 + 1);       Nop();
            associatedDevices[i].on_action = on_action;
        }

        deviceModChecksum = AssociatedDevicesModChecksum();
        deviceFadeChecksum = AssociatedDevicesFadeChecksum();
        deviceOnActionChecksum = AssociatedDevicesOnActionChecksum();
    }
}

void UpdateNVMemory()
{
    ClrWdt();
    updEEstatus = false;

    if (channel == 2)
    {
        TRX_ChangeChannel(CONNECTOR_CHAN_2);
    }
    else
    {
        channel = 1;
        TRX_ChangeChannel(CONNECTOR_CHAN_1);
    }

    DataEEWrite(channel | (((WORD) statePostscaler) << 8), CHANNEL_AND_STATE_POSTSCALER_ADDR);
    DataEEWrite(repeater ? 1 : 0, REPEATER_ADDR);
    DataEEWrite((gatewayID >> 16), GATEWAYID_ADDR);
    DataEEWrite(gatewayID, GATEWAYID_ADDR + 1);
    DataEEWrite(defaultSetpoint | (invertDimIfNewStateZero ? 0x80 : 0), DEFAULT_SET_POINT_ADDR);
}

void UpdateNVDevices()
{
    int i = 0;
    ClrWdt();
    updEEdevices = false;
    DataEEWrite(deviceCount, DEVICECOUNT_AND_TX_POWER_ADDR);

    for (i = 0; i < deviceCount; i++)
    {
        DataEEWrite(associatedDevices[i].id >> 16, ASSOCIATEDDEVICES_ADDR + (i*2));
        DataEEWrite(associatedDevices[i].id, ASSOCIATEDDEVICES_ADDR + (i*2) + 1);
    }

    for (i = 0; i < deviceCount; i++) {
        WORD modAndFade = associatedDevices[i].mod | (associatedDevices[i].fade << 8);
        DataEEWrite(modAndFade, ASSOCIATEDDEVICES_ADDR + (MAX_DEVICES * 2) + i * 2);
        DataEEWrite(associatedDevices[i].on_action, ASSOCIATEDDEVICES_ADDR + (MAX_DEVICES * 2) + i * 2 + 1);
    }
}

int inline disable_timer_interrupts(void)
{
    int old_ipl;

   if (isSparrowOutletConn(MY_DEV_TYPE) || isEmergeConn(MY_DEV_TYPE))
    {
        SET_AND_SAVE_CPU_IPL(old_ipl, TIMER_INTERRUPT_PRIORITY_LEVEL_07);
    }
    else
    {
        SET_AND_SAVE_CPU_IPL(old_ipl, TIMER_INTERRUPT_PRIORITY_LEVEL_06);
    }

    return old_ipl;
}
void inline enable_timer_interrupts(int old_ipl)
{
    RESTORE_CPU_IPL(old_ipl);
}

//* return true if the gateway id that originated a command is one
//* that can be used for the command.  Generally, gateway ids of 0 are
//* illegal except for use on the manufacturing floor to configure
//* Local Vacancy Connectors

bool
is_legal_gw_id(DWORD id)
{
    return id != 0;
}

//* Return true if the gateway id is the local vacancy gateway id.
// Connectors registered to this gateway operate in local vacancy
// mode.
//*

bool inline
is_local_vacancy_gw_id(DWORD id)
{
    return id == 0x00FFFFF0;
}

/* Return true if the connector is operating in local vacancy/room-in-a-box
 * mode.  This is true if the connector is of the appropriate type and
 * if its owning gatewayID is set to the local vacancy gateway id
 *
*/

bool inline
is_local_vacancy_connector()
{
    return isLocalVacancyConn(MY_DEV_TYPE) && is_local_vacancy_gw_id(gatewayID);
}


/* Return true if the connector is currently managed by a gateway.  This is
 * true if the connector is not a local vacancy connector and its associated
 * gateway ID is not zero.
*/

bool inline
is_managed_connector()
{
    if ( !is_local_vacancy_connector() && gatewayID != 0 ) {
	    return true;
    } else {
	    return false;
    }
}

bool inline
is_unowned()
{
    return gatewayID == 0;
}

void
update_connector_checksums()
{
    deviceChecksum = AssociatedDevicesChecksum();
    deviceModChecksum = AssociatedDevicesModChecksum();
    deviceFadeChecksum = AssociatedDevicesFadeChecksum();
    deviceOnActionChecksum = AssociatedDevicesOnActionChecksum();
    deviceSceneSwitchChecksum = AssociatedDevicesSceneSelectChecksum();
    updEEdevices = true;
}

//*********************************************************************
// timer1 interrupt routine
//*********************************************************************
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
    if( radioCalTimer != 0xffff )
      radioCalTimer++;
    if( radioResetTimer != 0xffff )
      radioResetTimer++;
    if ( !connectorHasBeenPolled && startupHeartbeatTimer != 0xffff)
        startupHeartbeatTimer++;
    vacancyTimer++;
    flashLightTimer++;

    if (startupDimDelay == 0)
        startupDimDelay++;  // only do this once for Emerge slower PWM connector

    // If in f/w upgrade mode bump timer and check for time-out
    if (fwUpgradeTimer != 0)  // TO DO - maybe make use an unused h/w timer and read value from chip instead of saving in RAM
        fwUpgradeTimer--;

    if (newOutputLevel < 1 && offDimLevelCounter < 2) {
        // wait a minimum of 1 full second before turning the voltage up
        // look for our second hit in this routine
        if (offDimLevelCounter == 1)
            if ( invertDimIfNewStateZero ) {
                OC1RS = 600; // Inverted output, set to 0V
            } else {
                OC1RS = 0;  // Normal (set to 10V)
            }
        else
            offDimLevelCounter++;
    }

    if (relayRisingCountdown > 0)
        relayRisingCountdown--;

    if (gatewayDisassociateTimeout > 0)
        gatewayDisassociateTimeout--;

    _T1IF = 0;  // Clear Timer1 interrupt flag last
}

// T5 not enabled in hardwareProfile.c if Sparrow/Outlet or Emerge connector which use momentary relays
void __attribute__((interrupt, auto_psv)) _T5Interrupt(void)
{
    // Turn Off Relays
    deenergize_bipin_relay(MY_DEV_TYPE);

    // Turn off timer
    T5CONbits.TON = 0;

    _T5IF = 0;
}
