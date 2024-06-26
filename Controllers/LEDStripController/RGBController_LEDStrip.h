/*---------------------------------------------------------*\
| RGBController_LEDStrip.h                                  |
|                                                           |
|   RGBController for serial LED strips                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      20 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "serial_port.h"
#include "LEDStripController.h"

class RGBController_LEDStrip : public RGBController
{
public:
    RGBController_LEDStrip(LEDStripController* controller_ptr);
    ~RGBController_LEDStrip();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LEDStripController* controller;
};
