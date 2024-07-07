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
  // Each frame is 16 bytes, to match the wMaxPacketSize of 16

  // The first byte contains report info.
  // The least significant 2 bits are the LED color channel. (0-2 R,G,B)
  // The 3 most significant bits are the LED row index (0-5) w/bar.
  // The remaining 15 bytes are the LED data.

  if (frame_count == 45) { // LED BAR
    // first 15 bytes are the R channel
    // second 15 bytes are the G channel
    // third 15 bytes are the B channel
    for (int channel = 0; channel < 3; channel++) {
      unsigned char buf[16] = {0};
      if (channel == 0) {
        buf[0] = 0b00000000;
      } else if (channel == 1) {
        buf[0] = 0b00000001;
      } else if (channel == 2) {
        buf[0] = 0b00000010;
      }
      for (int col = 0; col < 15; col++) {
        buf[col + 1] = frame_data[col + (channel * 15)];
      }
      hid_write(dev, buf, 16);
    }
  }
  // Send a flush command to the device to update the LEDs
  unsigned char flushbuf[16] = {0};
  flushbuf[0] = 0b11111111;
  hid_write(dev, flushbuf, 16);
}
