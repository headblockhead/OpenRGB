#pragma once

#include "RGBController.h"
#include "SlabController.h"

class RGBController_Slab : public RGBController {
public:
  RGBController_Slab(SlabController *controller_ptr);
  ~RGBController_Slab();

  void SetupZones();

  void ResizeZone(int zone, int new_size);

  void DeviceUpdateLEDs();
  void UpdateZoneLEDs(int zone);
  void UpdateSingleLED(int led);

  void SetCustomMode();
  void DeviceUpdateMode();
  void DeviceSaveMode(); // If mode is direct, the LED state will be saved to
                         // the device.

private:
  SlabController *controller;
};
