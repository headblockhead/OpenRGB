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

  unsigned char buf[64] = {0};
  buf[0] = 0b00000000; // Report ID
  for (unsigned char j = 0; j < 63;
       j++) { // Keyboard part 1 (63 bytes) - 21 LEDs
    buf[j + 1] = frame_data[j];
  }
  hid_write(dev, buf, 64);

  buf[0] = 0b00000001; // Report ID
  for (unsigned char j = 0; j < 63;
       j++) { // Keyboard part 2 (63 bytes) - 21 LEDs
    buf[j + 1] = frame_data[j + 63];
  }
  hid_write(dev, buf, 64);

  buf[0] = 0b00000010; // Report ID
  for (unsigned char j = 0; j < 63;
       j++) { // Keyboard part 3 (63 bytes) - 21 LEDs
    buf[j + 1] = frame_data[j + 126];
  }
  hid_write(dev, buf, 64);

  buf[0] = 0b00000011; // Report ID
  for (unsigned char j = 0; j < 63;
       j++) { // Keyboard part 4 (63 bytes) - 21 LEDs
    buf[j + 1] = frame_data[j + 189];
  }
  hid_write(dev, buf, 64);

  buf[0] = 0b00000100; // Report ID
  for (unsigned char j = 0; j < 18;
       j++) { // Keyboard part 5 (18 bytes) - 6 LEDs
    buf[j + 1] = frame_data[j + 207];
  }
  for (unsigned char j = 0; j < 45; j++) {
    buf[j + 19] = frame_data[j + 225]; // LED bar (45 bytes) - 15 LEDs
  }
  hid_write(dev, buf, 64);
}
