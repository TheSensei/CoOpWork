/*********************************************************************
 * device_type_functions.c
 * Written by Mark Brasili
 * Copyright 2015
 * Confidential and Proprietary to Powercast Corporation
 ********************************************************************/

#include "device_type_functions.h"

/*****************************************************************************
  Function:
    bool inline isSmartConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    A smart connector is a connector that supports a bi directional
    relay for on-off logic and dimming for a high voltage AC fixture.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isSmartConn(BYTE devType)
{
    return ((devType == DEV_TYPE_SMART_CONNECTOR) ||
	    (devType == DEV_TYPE_CFL_CONNECTOR) ||
	    (devType == DEV_TYPE_CFL_METAL_CONNECTOR) ||
	    (devType == DEV_TYPE_347_CONNECTOR) ||
	    (devType == DEV_TYPE_COMPACT_ELV_DIM_CONNECTOR) ||
	    (devType == DEV_TYPE_COMPACT_LINE_DIM_CONNECTOR) ||
	    (devType == DEV_TYPE_20AMP_SMART_CONNECTOR));
}

/*****************************************************************************
  Function:
    bool inline isEmergeConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    An emerge connector is a low voltage light fixture controller use
    to control LED lights.  These are designed for 24 volt lighting
    applications.  These connectors have a level driven relay and full
    dimming capability.  The dimming output is analog, driven
    internally by a PWM output that is using a square wave operating
    at, roughly, 6 Khz.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/

bool inline isEmergeConn(BYTE devType)
{
    return ((devType == DEV_TYPE_EMERGE_CONNECTOR) ||
	    (devType == DEV_TYPE_EMERGE_DOWNLIGHT_CONNECTOR) ||
	    (devType == DEV_TYPE_EMERGE_SLOWER_PWM_CONNECTOR) ||
	    (devType == DEV_TYPE_EMERGE_LUMINAIRE));
}

/*****************************************************************************
  Function:
    bool inline isEmergeSlowerPwmConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    This is a type of Emerge connector that uses different dimming
    logic.  This connector uses a digital, PWM, output running at
    roughly 120 Hz which is used to drive fixtures that want a PWM
    input signal for setting their brightness.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/

bool inline isEmergeSlowerPwmConn(BYTE devType)
{
    return ((devType == DEV_TYPE_EMERGE_SLOWER_PWM_CONNECTOR));
}

/*****************************************************************************
  Function:
    bool inline isSparrowOutletConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    A sparrow connector is a on-off relay connector with no dimming
    capability.  The relay is controlled by a level sensitive pin.
    when driven high by the PIC the relay is closed, when driven low,
    the relay is open.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isSparrowOutletConn(BYTE devType)
{
    return (devType == DEV_TYPE_SPARROW_OUTLET_CONNECTOR);
}


/*****************************************************************************
  Function:
    bool inline isAnyConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    This function returns true if a device is a connector.  The
    intended use is for distinguishing between connectors and other
    devices in code common to all devices, such as the radio code.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isAnyConn(BYTE devType)
{
    return (isSmartConn(devType) ||
	    isEmergeConn(devType) ||
	    isSparrowOutletConn(devType));
}

/*****************************************************************************
  Function:
    bool inline isBiPinRelayConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    A BiPinRelayConn is a connector that has a bistable latching relay.
    Instead of setting a level to turn the relay on or off, one needs to
    pulse one of two lines.  One drives the relay to the on state, the other
    drives the relay to the off state.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isBiPinRelayConn(BYTE devType)  // check for bi-stable relay else a momentary relay
{
    return (isSmartConn(devType));
}


/*****************************************************************************
  Function:
    bool inline isLocalVacancyConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    A local vacancy connector is a connector that can be paired with a
    motion sensor to operate independently of the Gateway.  Normally,
    motion sensors feed into the gateway which, in turn, drives the state
    of the connector.  These connectors are capable of listening directly
    to the motion sensor, thus they operate on changes in room vacancy
    without the direct intervention of the gateway.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isLocalVacancyConn(BYTE devType)
{
    return (isSparrowOutletConn(devType));
}

/*****************************************************************************
  Function:
    bool inline isDimmingConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    A dimming connector is a connector that has the capability of both
    turning a fixture on and off and setting the output level of the
    fixture.  A non-dimming connector is a connector that has a simple
    on-off output and cannot adjust the light level of the fixture
    other than by turning the fixture on or off.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isDimmingConn(BYTE devType)
{
    return (isSmartConn(devType) || isEmergeConn(devType));
}

/*****************************************************************************
  Function:
    bool inline isFlashLightConn(BYTE devType)

  Description:
    Checks if the device type passed is in this group of device types

    A Flash light connector is one that is capable of dimming so that
    the gateway can momentarily set the connector to full brightness
    as a warning that the room is about to time out.

  Parameters:
    devType - device type

  Returns:
    true - if passed device type is part of this group
    false - if passed device type is not part of this group of devices
  ***************************************************************************/
bool inline isFlashLightConn(BYTE devType)
{
    return (isSmartConn(devType) || isEmergeConn(devType));
}

/*****************************************************************************
 * Function:
 *     bool inline isRevBiPinRelayConn(BYTE devType)
 *
 * Description:
 *     Returns a TRUE value if the connector is a bistable relay device
 *     and it uses reverse connections to the Bi-Stable relay.
 *
 * Parameters:
 *     devType - device type
 *
 * Returns:
 *     true  - device is a bi-pin relay with reversed connections
 *     false - device is not a bi-pin relay or is a bi-pin relay
 *             that uses normal connections
 *
 * NOTE - consider renaming this to be
 *
 *  bool inline isTimTunnelSpecial(BYTE devType)
 *
 ****************************************************************************/

bool inline isRevBiPinRelayConn(BYTE devType)
{
    return devType == DEV_TYPE_20AMP_SMART_CONNECTOR;
}

/*****************************************************************************
 * Function:
 *     bool inline isLineDimmingConn(BYTE devType)
 *
 * Description:
 *     Returns a TRUE value if the connector dims the fixture by
 *     modulation of the power as opposed to the fixture having
 *     a separate dimming control input.
 *
 * Parameters:
 *     devType - device type
 *
 * Returns:
 *     true  - device dims by modulating power
 *     false - device does not support dimming or uses a separate
 *             control signal for dimming.
 *
 ****************************************************************************/

bool inline isLineDimmingConn(BYTE devType)
{
    return (devType == DEV_TYPE_COMPACT_LINE_DIM_CONNECTOR) ||
           (devType == DEV_TYPE_COMPACT_ELV_DIM_CONNECTOR);
}
