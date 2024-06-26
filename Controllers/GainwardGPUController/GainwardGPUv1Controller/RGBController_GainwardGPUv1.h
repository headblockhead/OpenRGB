/*---------------------------------------------------------*\
| RGBController_GainwardGPUv1.h                             |
|                                                           |
|   RGBController for Gainward v1 GPU                       |
|                                                           |
|   TheRogueZeta                                05 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GainwardGPUv1Controller.h"

class RGBController_GainwardGPUv1 : public RGBController
{
public:
    RGBController_GainwardGPUv1(GainwardGPUv1Controller* controller_ptr);
    ~RGBController_GainwardGPUv1();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GainwardGPUv1Controller* controller;

    int        GetDeviceMode();
};
