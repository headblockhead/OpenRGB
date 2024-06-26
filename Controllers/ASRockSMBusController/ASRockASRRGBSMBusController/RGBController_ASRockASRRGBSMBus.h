/*---------------------------------------------------------*\
| RGBController_ASRRGBSMBus.h                               |
|                                                           |
|   RGBController for SMBus ASRock ASR RGB motherboards     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockASRRGBSMBusController.h"

class RGBController_ASRockASRRGBSMBus : public RGBController
{
public:
    RGBController_ASRockASRRGBSMBus(ASRockASRRGBSMBusController* controller_ptr);
    ~RGBController_ASRockASRRGBSMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ASRockASRRGBSMBusController* controller;
};
