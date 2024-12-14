#include "SlabController.h"
#include "LogManager.h"

using namespace std::chrono_literals;

SlabController::SlabController(hid_device *dev_handle, const char *path) {
  dev = dev_handle;
  location = path;
}

SlabController::~SlabController() { hid_close(dev); }

std::string SlabController::GetDeviceLocation() { return ("HID: " + location); }

std::string SlabController::GetSerialString() {
  wchar_t serial_string[128];
  int ret = hid_get_serial_number_string(dev, serial_string, 128);

  if (ret != 0) {
    return ("");
  }

  std::wstring return_wstring = serial_string;
  std::string return_string(return_wstring.begin(), return_wstring.end());

  return (return_string);
}

void SlabController::SendDirect(unsigned int frame_count,
                                unsigned char *frame_data) {
  // Each frame is 64 bytes, to match the wMaxPacketSize of 64 for the USB HID
  // endpoint.
  if (frame_count == 0) {
    unsigned char buf[2] = {0}; // Report ID 0x00
    hid_write(dev, buf, 2);
    return;
  }
  for (unsigned int i = 0; i < frame_count; i++) {
    unsigned char buf[61] = {0};
    buf[0] = 0b00000001 + i; // Report ID
    for (unsigned char j = 0; j < 60; j++) {
      buf[j + 1] = frame_data[j + (60 * i)];
    }
    hid_write(dev, buf, 61);
  }
}

void SlabController::SendMode(unsigned int mode) {
  unsigned char buf[2] = {0};
  buf[0] = 0x00; // Report ID 0x03
  buf[1] = mode;
  hid_write(dev, buf, 2);
  //  std::this_thread::sleep_for(100ms);
};

void SlabController::SaveLEDState() {
  unsigned char buf[2] = {0};
  buf[0] = 0x01; // Report ID 0x01
  hid_write(dev, buf, 2);
};

void SlabController::SaveMode() {
  unsigned char buf[2] = {0};
  buf[0] = 0x02; // Report ID 0x02
  hid_write(dev, buf, 2);
};
