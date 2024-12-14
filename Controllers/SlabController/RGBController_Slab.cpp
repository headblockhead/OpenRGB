#include "RGBController_Slab.h"
#include "LogManager.h"
#include "RGBControllerKeyNames.h"

/**------------------------------------------------------------------*\
    @name Slab
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :x:
    @detectors Slab
    @comment headblockhead's Slab keyboard
\*-------------------------------------------------------------------*/

static unsigned int matrix_map[5][15] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
    {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44},
    {45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
    {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74}};

RGBController_Slab::RGBController_Slab(SlabController *controller_ptr) {
  controller = controller_ptr;

  name = "Slab";
  vendor = "headblockhead";
  type = DEVICE_TYPE_KEYBOARD;
  description = "headblockhead's Slab keyboard";
  location = controller->GetDeviceLocation();
  serial = controller->GetSerialString();

  mode Direct;
  Direct.name = "Direct";
  Direct.value = 0;
  Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS |
                 MODE_FLAG_MANUAL_SAVE;
  Direct.brightness_min = 0;
  Direct.brightness = 25;
  Direct.brightness_max = 100;
  Direct.color_mode = MODE_COLORS_PER_LED;
  modes.push_back(Direct);

  mode Reactive;
  Reactive.name = "Reactive";
  Reactive.value = 1;
  Reactive.flags = MODE_FLAG_MANUAL_SAVE;
  Reactive.color_mode = MODE_COLORS_NONE;
  modes.push_back(Reactive);

  SetupZones();
}

RGBController_Slab::~RGBController_Slab() { delete controller; }

void RGBController_Slab::SetupZones() {
  zone Bar;
  Bar.name = "Bar";
  Bar.type = ZONE_TYPE_LINEAR;
  Bar.leds_min = 15;
  Bar.leds_max = 15;
  Bar.leds_count = 15;
  Bar.matrix_map = NULL;

  for (int i = 0; i < 15; i++) {
    led new_led;
    new_led.name = "BLED " + std::to_string(i);
    new_led.value = i;
    leds.push_back(new_led);
  }

  zone Keyboard;
  Keyboard.name = ZONE_EN_KEYBOARD;
  Keyboard.type = ZONE_TYPE_MATRIX;
  Keyboard.leds_min = 75;
  Keyboard.leds_max = 75;
  Keyboard.leds_count = 75;
  Keyboard.matrix_map = new matrix_map_type;
  Keyboard.matrix_map->height = 5;
  Keyboard.matrix_map->width = 15;
  Keyboard.matrix_map->map = (unsigned int *)&matrix_map;

  for (int i = 0; i < 75; i++) {
    led new_led;
    new_led.name = "LED " + std::to_string(i);
    new_led.value = i;
    leds.push_back(new_led);
  }

  zones.push_back(Keyboard);
  zones.push_back(Bar);
  SetupColors();
}

void RGBController_Slab::ResizeZone(int /*zone*/, int /*new_size*/) {
  // Unsupported
}

void RGBController_Slab::DeviceUpdateLEDs() {
  unsigned char frame_data[360] = {
      0}; // 360bytes is the worst possible data transmission.
  int data_index = 0;
  // run length encode the LED data
  for (int i = 0; i < 90; i++) { // 90 LEDs
    unsigned char r = RGBGetRValue(colors[i]);
    unsigned char g = RGBGetGValue(colors[i]);
    unsigned char b = RGBGetBValue(colors[i]);
    uint32_t color = r << 16 | g << 8 | b;
    unsigned char count = 1;
    while (i < 89 && color == (RGBGetRValue(colors[i + 1]) << 16 |
                               RGBGetGValue(colors[i + 1]) << 8 |
                               RGBGetBValue(colors[i + 1]))) {
      count++;
      i++;
    }
    frame_data[data_index++] = count;
    frame_data[data_index++] = r * modes[active_mode].brightness / 100;
    frame_data[data_index++] = g * modes[active_mode].brightness / 100;
    frame_data[data_index++] = b * modes[active_mode].brightness / 100;
  }
  controller->SendDirect((data_index + (61 - 1)) / 61, frame_data);
  controller->SendDirect(0, frame_data);
}

void RGBController_Slab::UpdateZoneLEDs(int /*zone*/) { DeviceUpdateLEDs(); }

void RGBController_Slab::UpdateSingleLED(int /*led*/) { DeviceUpdateLEDs(); }

void RGBController_Slab::SetCustomMode() {}

void RGBController_Slab::DeviceUpdateMode() {
  controller->SendMode(modes[active_mode].value);
  if (modes[active_mode].value == 0) { // Direct Mode
    DeviceUpdateLEDs();
  }
}

void RGBController_Slab::DeviceSaveMode() {
  DeviceUpdateMode();
  if (modes[active_mode].value == 0) { // Direct Mode
    controller->SaveLEDState();
    return;
  }
  controller->SaveMode();
}
