#include "Detector.h"
#include "RGBController.h"
#include <hidapi/hidapi.h>

#include "LogManager.h"
#include "RGBController_Slab.h"
#include "SlabController.h"
#include <stdlib.h>

#define SLAB_VID 0x4EAD
#define SLAB_PID 0x57AB

void DetectSlabControllers(hid_device_info *info, const std::string &) {
  hid_device *dev = hid_open_path(info->path);

  if (dev) {
    SlabController *controller = new SlabController(dev, info->path);
    RGBController_Slab *rgb_controller = new RGBController_Slab(controller);
    // Constructor sets the name
    ResourceManager::get()->RegisterRGBController(rgb_controller);
  } else {
    LOG_ERROR("SlabControllerDetect.cpp: Failed to open Slab controller");
    const wchar_t *err = hid_error(NULL);
    char err2[1024];
    wcstombs(err2, err, 1024);
  }
}

REGISTER_HID_DETECTOR_IPU("Slab", DetectSlabControllers, SLAB_VID, SLAB_PID, 0,
                          1, 6);
