/*---------------------------------------------------------*\
| RGBController_HoltekA070.h                                |
|                                                           |
|   RGBController for Holtek mouse                          |
|                                                           |
|   Santeri Pikarinen (santeri3700)             01 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HoltekA070Controller.h"

class RGBController_HoltekA070 : public RGBController
{
public:
    RGBController_HoltekA070(HoltekA070Controller* controller_ptr);
    ~RGBController_HoltekA070();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HoltekA070Controller* controller;
};
