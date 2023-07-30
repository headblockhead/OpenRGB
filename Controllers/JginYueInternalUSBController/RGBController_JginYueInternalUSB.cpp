/*------------------------------------------------*\
|  RGBController_JginYueInternalUSB.cpp            |
|                                                  |
|  Generic RGB Interface JginYueInternalUSB Class  |
|                                                  |
|  Adam Honse (CalcProgrammer1) 2/25/2020          |
|  Adam Honse (CalcProgrammer1) 2/25/2020          |
\*------------------------------------------------*/

#include "RGBController_JginYueInternalUSB.h"
#include <string.h>

#define JGINYUE_MAX_ZONES               2
#define JGINYUE_ADDRESSABLE_MAX_LEDS    100
/**------------------------------------------------------------------*\
    @name JginYueInternalUSB
    @category JginYue devices
    @type  USB
    @save :white_check_mark: or :robot: or :o: or :x:
    @direct :white_check_mark: or :rotating_light: or :o: or :x:
    @effects :white_check_mark: or :rotating_light: or :tools: or :o: or :x:
    @detectors DetectJginYueInternalUSB,DetectJginYueInternalUSB2
    @comment Insert multiline JginYueInternalUSB comment here

        | Symbol | Meaning |
        | :---: | :--- |
        | :white_check_mark: | Fully supported by OpenRGB |
        | :rotating_light: | Support is problematic |
        | :robot: | Feature is automatic and can not be turned off |
        | :tools: | Partially supported by OpenRGB |
        | :o: | Not currently supported by OpenRGB |
        | :x: | Not applicable for this device |
*/

RGBController_JginYueInternalUSB::RGBController_JginYueInternalUSB(JginYueInternalUSBController* controller_ptr)
{
    controller                      =controller_ptr;

    name                            =controller->GetDeviceName();
    description                     ="JGINYUE USB ARGB Device";
    vendor                          ="JGINYUE";
    type                            =DEVICE_TYPE_MOTHERBOARD;
    location                        =controller->GetDeviceLocation();
    version                         =controller->GetDeviceFWVirson();

    mode Off;
    Off.name                        ="Off";
    Off.value                       =JGINYUE_USB_MODE_OFF;
    Off.flags                       =0;
    Off.color_mode                  =MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                     ="Static";
    Static.value                    =JGINYUE_USB_MODE_STATIC;
    Static.flags                    =MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode               =MODE_COLORS_MODE_SPECIFIC;
    Static.colors_max               =1;
    Static.colors_min               =1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  ="Breathing";
    Breathing.value                 =JGINYUE_USB_MODE_BREATHING;
    Breathing.flags                 =MODE_FLAG_HAS_MODE_SPECIFIC_COLOR|MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode            =MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_max            =1;
    Breathing.colors_min            =1;
    Breathing.brightness            =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Breathing.brightness_max        =JGINYUE_USB_BRIGHTNESS_MAX;
    Breathing.brightness_min        =JGINYUE_USB_BRIGHTNESS_MIN;
    Breathing.speed                 =JGINYUE_USB_SPEED_DEFAULT;
    Breathing.speed_max             =JGINYUE_USB_SPEED_MAX;
    Breathing.speed_min             =JGINYUE_USB_SPEED_MIN;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                     ="Strobe";
    Strobe.value                    =JGINYUE_USB_MODE_STROBE;
    Strobe.flags                    =MODE_FLAG_HAS_MODE_SPECIFIC_COLOR|MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS;
    Strobe.color_mode               =MODE_COLORS_MODE_SPECIFIC;
    Strobe.colors_max               =1;
    Strobe.colors_min               =1;
    Strobe.brightness               =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Strobe.brightness_max           =JGINYUE_USB_BRIGHTNESS_MAX;
    Strobe.brightness_min           =JGINYUE_USB_BRIGHTNESS_MIN;
    Strobe.speed                    =JGINYUE_USB_SPEED_DEFAULT;
    Strobe.speed_max                =JGINYUE_USB_SPEED_MAX;
    Strobe.speed_min                =JGINYUE_USB_SPEED_MIN;
    Strobe.colors.resize(1);
    modes.push_back(Strobe);

    mode Cycling;
    Cycling.name                    ="Cycling";
    Cycling.value                   =JGINYUE_USB_MODE_CYCLING;
    Cycling.flags                   =MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS;
    Cycling.color_mode              =MODE_COLORS_NONE;
    Cycling.brightness              =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Cycling.brightness_max          =JGINYUE_USB_BRIGHTNESS_MAX;
    Cycling.brightness_min          =JGINYUE_USB_BRIGHTNESS_MIN;
    Cycling.speed                   =JGINYUE_USB_SPEED_DEFAULT;
    Cycling.speed_max               =JGINYUE_USB_SPEED_MAX;
    Cycling.speed_min               =JGINYUE_USB_SPEED_MIN;
    modes.push_back(Cycling);

    mode Random;
    Random.name                     ="Random";
    Random.value                    =JGINYUE_USB_MODE_RANDOM;
    Random.flags                    =MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS;
    Random.color_mode               =MODE_COLORS_NONE;
    Random.brightness               =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Random.brightness_max           =JGINYUE_USB_BRIGHTNESS_MAX;
    Random.brightness_min           =JGINYUE_USB_BRIGHTNESS_MIN;
    Random.speed                    =JGINYUE_USB_SPEED_DEFAULT;
    Random.speed_max                =JGINYUE_USB_SPEED_MAX;
    Random.speed_min                =JGINYUE_USB_SPEED_MIN;
    modes.push_back(Random);

    mode Wave;
    Wave.name                       ="Wave";
    Wave.value                      =JGINYUE_USB_MODE_WAVE;
    Wave.flags                      =MODE_FLAG_HAS_SPEED;
    Wave.color_mode                 =MODE_COLORS_NONE;
    Wave.speed                      =JGINYUE_USB_SPEED_DEFAULT;
    Wave.speed_max                  =JGINYUE_USB_SPEED_MAX;
    Wave.speed_min                  =JGINYUE_USB_SPEED_MIN;
    modes.push_back(Wave);

    mode Spring;
    Spring.name                     ="Spring";
    Spring.value                    =JGINYUE_USB_MODE_SPRING;
    Spring.flags                    =MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS|MODE_FLAG_HAS_DIRECTION_LR;
    Spring.color_mode               =MODE_COLORS_NONE;
    Spring.brightness               =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Spring.brightness_max           =JGINYUE_USB_BRIGHTNESS_MAX;
    Spring.brightness_min           =JGINYUE_USB_BRIGHTNESS_MIN;
    Spring.speed                    =JGINYUE_USB_SPEED_DEFAULT;
    Spring.speed_max                =JGINYUE_USB_SPEED_MAX;
    Spring.speed_min                =JGINYUE_USB_SPEED_MIN;
    Spring.direction                =0x00;
    modes.push_back(Spring);

    mode Water;
    Water.name                      ="Water";
    Water.value                     =JGINYUE_USB_MODE_WATER;
    Water.flags                     =MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS|MODE_FLAG_HAS_DIRECTION_LR;
    Water.color_mode                =MODE_COLORS_NONE;
    Water.brightness                =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Water.brightness_max            =JGINYUE_USB_BRIGHTNESS_MAX;
    Water.brightness_min            =JGINYUE_USB_BRIGHTNESS_MIN;
    Water.speed                     =JGINYUE_USB_SPEED_DEFAULT;
    Water.speed_max                 =JGINYUE_USB_SPEED_MAX;
    Water.speed_min                 =JGINYUE_USB_SPEED_MIN;
    Water.direction                 =MODE_DIRECTION_RIGHT;
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                    ="Rainbow";
    Rainbow.value                   =JGINYUE_USB_MODE_RAINBOW;
    Rainbow.flags                   =MODE_FLAG_HAS_SPEED|MODE_FLAG_HAS_BRIGHTNESS|MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode              =MODE_COLORS_NONE;
    Rainbow.brightness              =JGINYUE_USB_BRIGHTNESS_DEFAULT;
    Rainbow.brightness_max          =JGINYUE_USB_BRIGHTNESS_MAX;
    Rainbow.brightness_min          =JGINYUE_USB_BRIGHTNESS_MIN;
    Rainbow.speed                   =JGINYUE_USB_SPEED_DEFAULT;
    Rainbow.speed_max               =JGINYUE_USB_SPEED_MAX;
    Rainbow.speed_min               =JGINYUE_USB_SPEED_MIN;
    Rainbow.direction               =MODE_DIRECTION_RIGHT;
    modes.push_back(Rainbow);

    mode Direct;
    Direct.name                     ="Direct";
    Direct.value                    =JGINYUE_USB_MODE_DIRECT;
    Direct.flags                    =MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               =MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}




void RGBController_JginYueInternalUSB::SetupZones()
{
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    leds.clear();
    colors.clear();
    zones.resize(JGINYUE_MAX_ZONES);


    zones[0].name       = "ARGB_Header_1";
    zones[0].type       = ZONE_TYPE_LINEAR;
    zones[0].leds_min   = 1;
    zones[0].leds_max   = 100;
    zones[0].matrix_map = NULL;

    zones[1].name       = "ARGB_Header_2";
    zones[1].type       = ZONE_TYPE_LINEAR;
    zones[1].leds_min   = 1;
    zones[1].leds_max   = 100;
    zones[1].matrix_map = NULL;

    if(first_run)
    {
        zones[0].leds_count=100;
        zones[1].leds_count=100;
    }

    for (int led_idx = 0; led_idx < zones[0].leds_count + zones[1].leds_count ; led_idx++)
    {
        int j=1;
        if(led_idx==zones[0].leds_count) j=1;

        if (led_idx<zones[0].leds_count)
        {
            led ARGB_1_HEADER;
            ARGB_1_HEADER.name    = "ARGB_HEADER_1_LED No." + std::to_string(j);
            ARGB_1_HEADER.value   = 0;
            leds.push_back(ARGB_1_HEADER);
        }
        else
        {
            led ARGB_2_HEADER;
            ARGB_2_HEADER.name    = "ARGB_HEADER_2_LED No." + std::to_string(j);
            ARGB_2_HEADER.value   = 1;
            leds.push_back(ARGB_2_HEADER);
        }
    }
    SetupColors();
}

void RGBController_JginYueInternalUSB::ResizeZone(int zone, int new_size)
{
    unsigned char area;
    switch (zone)
    {
    case 0:
        area = 0x01;
        break;
    case 1:
        area = 0x02;
        break;
    default:
        break;
    }
    SetupZones();
    controller->Area_resize(new_size,area);
}

void RGBController_JginYueInternalUSB::DeviceUpdateLEDs()
{
    for (int i = 1; i <= JGINYUE_MAX_ZONES; i++)
    {
        UpdateZoneLEDs(i);
    }

}

void RGBController_JginYueInternalUSB::UpdateZoneLEDs(int zone)
{
    unsigned char area;
    switch (zone)
    {
    case 0:
        area = 0x01;
        break;
    case 1:
        area = 0x02;
        break;
    default:
        break;
    }
    controller->DirectLEDControl(zones[zone].colors,area);
}

void RGBController_JginYueInternalUSB::UpdateSingleLED(int led)
{
    int zone;
    zone = leds[led].value;
    UpdateZoneLEDs(zone);
}


void RGBController_JginYueInternalUSB::DeviceUpdateMode()
{

}
