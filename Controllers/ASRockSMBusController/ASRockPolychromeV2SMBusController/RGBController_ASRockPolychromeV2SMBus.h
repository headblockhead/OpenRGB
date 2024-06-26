/*---------------------------------------------------------*\
| RGBController_ASRockPolychromeV2SMBus.h                   |
|                                                           |
|   RGBController for SMBus ASRock Polychrome V2            |
|   motherboards                                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockPolychromeV2SMBusController.h"

class RGBController_ASRockPolychromeV2SMBus : public RGBController
{
public:
    RGBController_ASRockPolychromeV2SMBus(ASRockPolychromeV2SMBusController* controller_ptr);
    ~RGBController_ASRockPolychromeV2SMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ASRockPolychromeV2SMBusController* controller;
};
