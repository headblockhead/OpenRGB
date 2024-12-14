#pragma once

#include "RGBController.h"
#include <hidapi/hidapi.h>

class SlabController {
public:
  SlabController(hid_device *dev_handle, const char *path);
  ~SlabController();

  std::string GetDeviceLocation();
  std::string GetSerialString();

  void SendDirect(unsigned int frame_count, unsigned char *frame_data);
  void SendMode(unsigned int mode);
  void SaveLEDState();
  void SaveMode();

private:
  hid_device *dev;
  std::string location;
};
