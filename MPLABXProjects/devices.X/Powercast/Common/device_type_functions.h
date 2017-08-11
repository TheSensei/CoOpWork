/*********************************************************************
 * device_type_functions.h
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#ifndef DEV_TYPE_FUNCTIONS_H
#define	DEV_TYPE_FUNCTIONS_H

#include "GenericTypeDefs.h"
#include "stdbool.h"

// These types should match to the types defined in the latest "Lighting System Specification":
#define DEV_TYPE_GATEWAY 0x00
#define DEV_TYPE_SMART_CONNECTOR 0x01
#define DEV_TYPE_CFL_CONNECTOR 0x02
#define DEV_TYPE_347_CONNECTOR 0x03
#define DEV_TYPE_SMART_SWITCH 0x04
#define DEV_TYPE_CEILING_MOTION_SENSOR 0x05
#define DEV_TYPE_WALL_MOTION_SENSOR 0x06
#define DEV_TYPE_LIGHT_SENSOR 0x07
#define DEV_TYPE_SPARROW_OUTLET_CONNECTOR 0x08
#define DEV_TYPE_EMERGE_CONNECTOR 0x09
#define DEV_TYPE_FLUSHMOUNT_SWITCH 0x0A
#define DEV_TYPE_EMERGE_DOWNLIGHT_CONNECTOR 0x0B
#define DEV_TYPE_FLUSHMOUNT_SCENE_SWITCH 0x0C
#define DEV_TYPE_SCENE_SWITCH 0x0D
#define DEV_TYPE_LVDC_ON_GRID 0x0E
#define DEV_TYPE_WALL_ONOFF_ONLY_SWITCH 0x0F
#define DEV_TYPE_COMPACT_LINE_DIM_CONNECTOR 0x10
#define DEV_TYPE_COMPACT_ELV_DIM_CONNECTOR 0x11
#define DEV_TYPE_CFL_METAL_CONNECTOR 0x12
#define DEV_TYPE_20AMP_SMART_CONNECTOR 0x1B
#define DEV_TYPE_EMERGE_LUMINAIRE 0x1C

// previous source control folder name is "Smart Connector - Emerge - Slower PWM"
#define DEV_TYPE_EMERGE_SLOWER_PWM_CONNECTOR  DEV_TYPE_LVDC_ON_GRID


#define DEV_TYPE_ROOM 0xFF

bool inline isSmartConn(BYTE devType);
bool inline isEmergeConn(BYTE devType);
bool inline isEmergeSlowerPwmConn(BYTE devType);
bool inline isSparrowOutletConn(BYTE devType);
bool inline isAnyConn(BYTE devType);
bool inline isBiPinRelayConn(BYTE devType);
bool inline isLocalVacancyConn(BYTE devType);
bool inline isDimmingConn(BYTE devType);
bool inline isFlashLightConn(BYTE devType);
bool inline isRevBiPinRelayConn(BYTE devType);
bool inline isLineDimmingConn(BYTE devType);

#endif	/* DEV_TYPE_FUNCTIONS_H */
