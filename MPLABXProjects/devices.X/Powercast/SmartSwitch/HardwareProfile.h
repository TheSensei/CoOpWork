/*********************************************************************
 * SmartSwitch - Audacy Devices
 * HardwareProfile.h
 * Written by Daniel Harrist
 * Copyright 2016
 ********************************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "TimeDelay.h"

/************************ FUNCTION PROTOTYPES **********************/
void BoardInit(void);
void Power(int);
WORD ADC_read(int);

/***************** SWITCH CONFIGURATION ****************************/

// Cross reference from compiler macros to device type codes
//
//  Type         Code  What Buttons It Provides
// ------------------------------------------------
// REGULAR_WALL  0x04  ON, UP, DOWN, DIM
// REGULAR_FLUSH 0x0A  ON, UP, DOWN, DIM
// SCENE_FLUSH   0x0C  A, B, C, D
// SCENE_WALL    0x0D  A, B, C, D,
// ON_OFF_FLUSH  0x0F  ON, OFF

//#define REGULAR_FLUSH
//#define REGULAR_WALL
//#define SCENE_FLUSH
//#define SCENE_WALL
//#define ON_OFF_FLUSH

// In case we have to release software that operates with the V1 wall
// switches, define this macro.  The channel to slider mapping changed
// between v1 and v2. Defining this uses the old mapping.

//#define V1_WALL_SWITCH

#ifdef V1_WALL_SWITCH
#undef HAS_BUTTON
#else
#define HAS_BUTTON
#endif

#if \
!defined(REGULAR_WALL) && \
!defined(REGULAR_FLUSH) && \
!defined(SCENE_FLUSH) && \
!defined(SCENE_WALL) && \
!defined(ON_OFF_FLUSH)
#error  "Switch Configuration Not Defined"
#endif

#if defined(REGULAR_WALL)
#if \
defined(REGULAR_FLUSH) || \
defined(SCENE_FLUSH) || \
defined(SCENE_WALL) || \
defined(ON_OFF_FLUSH)
#error  "Multiple Configurations Defined"
#endif
#endif

#if defined(REGULAR_FLUSH)
#if \
defined(REGULAR_WALL) || \
defined(SCENE_FLUSH) || \
defined(SCENE_WALL) || \
defined(ON_OFF_FLUSH)
#error  "Multiple Configurations Defined"
#endif
#endif

#if defined(SCENE_FLUSH)
#if \
defined(REGULAR_FLUSH) || \
defined(REGULAR_WALL) || \
defined(SCENE_WALL) || \
defined(ON_OFF_FLUSH)
#error  "Multiple Configurations Defined"
#endif
#endif

#if defined(SCENE_WALL)
#if \
defined(REGULAR_FLUSH) || \
defined(SCENE_FLUSH) || \
defined(REGULAR_WALL) || \
defined(ON_OFF_FLUSH)
#error  "Multiple Configurations Defined"
#endif
#endif

#if defined(ON_OFF_FLUSH)
#if \
defined(REGULAR_FLUSH) || \
defined(SCENE_FLUSH) || \
defined(SCENE_WALL) || \
defined(REGULAR_WALL)
#error  "Multiple Configurations Defined"
#endif
#endif

/************************ MACROS ***********************************/
#define POWER_UP                0
#define POWER_DOWN              1

#define CLOCK_FREQ              8000000
#define GetInstructionClock()   (CLOCK_FREQ/2)

// Radio Power Control
#define RADIO_PWR               _LATB15
#define RADIO_PWR_TRIS          _TRISB15

// Transceiver Configuration
#define TRX_INT_PIN             _RA6
#define TRX_INT_TRIS            _TRISA6
#define TRX_CSn                 _LATA4
#define TRX_CSn_TRIS            _TRISA4
#define TRX_RSTn                _LATB8
#define TRX_RSTn_TRIS           _TRISB8
#define TRX_SLP                 _LATB7
#define TRX_SLP_TRIS            _TRISB7
#define TRX_DIG2                _RB12
#define TRX_DIG2_TRIS           _TRISB12
#define TRX_IF                  _INT2IF
#define TRX_IE                  _INT2IE
#define TRX2_IE                 _INT1IE
#define TRX2_CSn                _LATA4
#define RADIO_INTERRUPT_PRIORITY_LEVEL 0x01

// LED pin
#define LED                     _LATA3
#define LED_TRIS                _TRISA3

// Sensor Power Control
#define SENS_PWR                _LATB4
#define SENS_PWR_TRIS           _TRISB4

// Battery Read Control
#define BATT_PWR                _LATA2
#define BATT_PWR_TRIS           _TRISA2

#define START_BTN               _RA7
#define START_BTN_TRIS          _TRISA7

// Analog channel set up for buttons

#define CHAN3_LAT               _LATB0
#define CHAN3_TRIS              _TRISB0
#define CHAN3_AN                _PCFG2
#define CHAN2_LAT               _LATB2
#define CHAN2_TRIS              _TRISB2
#define CHAN2_AN                _PCFG4
#define CHAN1_LAT               _LATB1
#define CHAN1_TRIS              _TRISB1
#define CHAN1_AN                _PCFG3

#if defined REGULAR_FLUSH || defined SCENE_FLUSH || defined ON_OFF_FLUSH || !defined V1_WALL_SWITCH
#define CHAN0_LAT               _LATA1
#define CHAN0_TRIS              _TRISA1
#define CHAN0_AN                _PCFG1
#define CHAN4_LAT               _LATA0
#define CHAN4_TRIS              _TRISA0
#define CHAN4_AN                _PCFG0
#elif defined REGULAR_WALL || defined SCENE_WALL && defined V1_WALL_SWITCH
#define CHAN0_LAT               _LATA0
#define CHAN0_TRIS              _TRISA0
#define CHAN0_AN                _PCFG0
#define CHAN4_LAT               _LATA1
#define CHAN4_TRIS              _TRISA1
#define CHAN4_AN                _PCFG1
#endif

// ADC inputs
#define REF_CHANNEL             10
#define BATT_CHANNEL            5
#define CHAN3_CHANNEL           2
#define CHAN2_CHANNEL           4
#define CHAN1_CHANNEL           3

#if defined REGULAR_FLUSH || defined SCENE_FLUSH || defined ON_OFF_FLUSH || !defined(V1_WALL_SWITCH)
#define CHAN0_CHANNEL           1
#define CHAN4_CHANNEL           0
#elif defined REGULAR_WALL || defined SCENE_WALL && defined V1_WALL_SWITCH
#define CHAN0_CHANNEL           0
#define CHAN4_CHANNEL           1
#endif

// Limit code size
#define TX_ONLY

// Button set up
#if defined REGULAR_FLUSH || defined SCENE_FLUSH || defined ON_OFF_FLUSH
    #define NUM_CTMU_CH         5   // 4 buttons, 1 dummy
    #define NUM_CTMU_SAMPS_PER_CHAN 4 // 4 samples for better noise filtering

    #define CHAN1_SENS          25  //30  // 20
    #define CHAN2_SENS          30  //34
    #define CHAN3_SENS          20  //30
    #define CHAN4_SENS          34  //30  //34
#elif defined REGULAR_WALL || defined SCENE_WALL
    #define NUM_CTMU_CH         5   // 4 buttons, 1 dummy
    #define NUM_CTMU_SAMPS_PER_CHAN 4 // 1 sample to save power

    #define CHAN1_SENS          20  //30
    #define CHAN2_SENS          20
    #define CHAN3_SENS          25
    #define CHAN4_SENS          30
#endif

// Button Commands to Send
#if defined REGULAR_FLUSH
    #define COMMAND_ONE         30  // UP
    #define COMMAND_TWO         10  // OFF
    #define COMMAND_THREE       20  // ON
    #define COMMAND_FOUR        40  // DOWN
#elif defined REGULAR_WALL
    #define COMMAND_ONE         10  // OFF
    #define COMMAND_TWO         40  // DOWN
    #define COMMAND_THREE       30  // UP
    #define COMMAND_FOUR        20  // ON
#elif defined SCENE_FLUSH
    #define COMMAND_ONE         35
    #define COMMAND_TWO         45
    #define COMMAND_THREE       15
    #define COMMAND_FOUR        25
#elif defined SCENE_WALL
    #define COMMAND_ONE         45
    #define COMMAND_TWO         35
    #define COMMAND_THREE       25
    #define COMMAND_FOUR        15
#elif defined ON_OFF_FLUSH
    #define COMMAND_ONE         0   // Heartbeat (will never send)
    #define COMMAND_TWO         10  // OFF
    #define COMMAND_THREE       20  // ON
    #define COMMAND_FOUR        0   // Heartbeat (will never send)
#endif
