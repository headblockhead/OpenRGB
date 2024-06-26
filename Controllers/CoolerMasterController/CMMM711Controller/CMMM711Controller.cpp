/*---------------------------------------------------------*\
| CMMM711Controller.cpp                                     |
|                                                           |
|   Driver for Cooler Master M711 mouse                     |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMMM711Controller.h"

CMMM711Controller::CMMM711Controller(hid_device* dev_handle, char *_path)
{
    const int szTemp = HID_MAX_STR;
    wchar_t tmpName[szTemp];

    dev                     = dev_handle;
    location                = _path;
    current_speed           = CM_MM711_SPEED_NORMAL;

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_indexed_string(dev, 2, tmpName, szTemp);
    wName = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

    SendInitPacket();
    GetColourStatus();
    GetCustomStatus();
    GetModeStatus();
}

CMMM711Controller::~CMMM711Controller()
{
    hid_close(dev);
}

void CMMM711Controller::GetColourStatus()
{
    uint8_t buffer[CM_MM711_PACKET_SIZE]    = { 0x00, 0x52, 0x2B };

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM711_PACKET_SIZE, CM_MM711_INTERRUPT_TIMEOUT);

    current_brightness      = buffer[CM_MM711_BRIGHTNESS_BYTE - 1];
    current_red             = buffer[CM_MM711_RED_BYTE - 1];
    current_green           = buffer[CM_MM711_GREEN_BYTE - 1];
    current_blue            = buffer[CM_MM711_BLUE_BYTE - 1];
}

void CMMM711Controller::GetCustomStatus()
{
    uint8_t buffer[CM_MM711_PACKET_SIZE]    = { 0x00, 0x52, 0xA8 };
    int read_size                           = CM_MM711_PACKET_SIZE - 1;
    int result                              = 0;

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    do
    {
        result = hid_read_timeout(dev, buffer, read_size, CM_MM711_INTERRUPT_TIMEOUT);
    }while(buffer[1] != 0xA8 && result == read_size);

    if(result == read_size)
    {
        wheel_colour            = ToRGBColor(buffer[4], buffer[5], buffer[6]);
        logo_colour             = ToRGBColor(buffer[7], buffer[8], buffer[9]);
    }
}

void CMMM711Controller::GetModeStatus()
{
    uint8_t buffer[CM_MM711_PACKET_SIZE]    = { 0x00, 0x52, 0x28 };
    int buffer_size                         = (sizeof(buffer) / sizeof(buffer[0]));

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_MM711_INTERRUPT_TIMEOUT);

    current_mode            = buffer[CM_MM711_MODE_BYTE - 1];
}

std::string CMMM711Controller::GetDeviceName()
{
    return(device_name);
}

std::string CMMM711Controller::GetSerial()
{
    return(serial);
}

std::string CMMM711Controller::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMMM711Controller::GetMode()
{
    return(current_mode);
}

unsigned char CMMM711Controller::GetLedRed()
{
    return(current_red);
}

unsigned char CMMM711Controller::GetLedGreen()
{
    return(current_green);
}

unsigned char CMMM711Controller::GetLedBlue()
{
    return(current_blue);
}

unsigned char CMMM711Controller::GetLedSpeed()
{
    return(current_speed);
}

RGBColor CMMM711Controller::GetWheelColour()
{
    return(wheel_colour);
}

RGBColor CMMM711Controller::GetLogoColour()
{
    return(logo_colour);
}

void CMMM711Controller::SetLedsDirect(RGBColor wheel_colour, RGBColor logo_colour)
{
    unsigned char buffer[CM_MM711_PACKET_SIZE]  = { 0x00, 0x51, 0xA8, 0x00, 0x00 };

    buffer[CM_MM711_MODE_BYTE]                  = RGBGetRValue(wheel_colour);
    buffer[CM_MM711_SPEED_BYTE]                 = RGBGetGValue(wheel_colour);
    buffer[CM_MM711_NFI_1]                      = RGBGetBValue(wheel_colour);
    buffer[CM_MM711_NFI_2]                      = RGBGetRValue(logo_colour);
    buffer[CM_MM711_NFI_3]                      = RGBGetGValue(logo_colour);
    buffer[CM_MM711_BRIGHTNESS_BYTE]            = RGBGetBValue(logo_colour);

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM711_PACKET_SIZE, CM_MM711_INTERRUPT_TIMEOUT);

    //SendApplyPacket(0xB0);  //Apply custom mode
}

void CMMM711Controller::SendUpdate(uint8_t mode, uint8_t speed, RGBColor colour, uint8_t brightness)
{
    unsigned char buffer[CM_MM711_PACKET_SIZE]  = { 0x00, 0x51, 0x2B, 0x00, 0x00 };

    buffer[CM_MM711_MODE_BYTE]                  = mode;
    buffer[CM_MM711_SPEED_BYTE]                 = speed;
    buffer[CM_MM711_NFI_1]                      = 0x20;
    buffer[CM_MM711_NFI_2]                      = 0xFF;
    buffer[CM_MM711_NFI_3]                      = 0xFF;
    buffer[CM_MM711_BRIGHTNESS_BYTE]            = brightness;
    buffer[CM_MM711_RED_BYTE]                   = RGBGetRValue(colour);
    buffer[CM_MM711_GREEN_BYTE]                 = RGBGetGValue(colour);
    buffer[CM_MM711_BLUE_BYTE]                  = RGBGetBValue(colour);

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM711_PACKET_SIZE, CM_MM711_INTERRUPT_TIMEOUT);

    SendApplyPacket(mode);
}

void CMMM711Controller::SendInitPacket()
{
    unsigned char buffer[CM_MM711_PACKET_SIZE]  = { 0x00, 0x41, 0x80 };

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM711_PACKET_SIZE, CM_MM711_INTERRUPT_TIMEOUT);
}

void CMMM711Controller::SendApplyPacket(uint8_t mode)
{
    unsigned char buffer[CM_MM711_PACKET_SIZE]  = { 0x00, 0x51, 0x28, 0x00, 0x00 };

    buffer[CM_MM711_MODE_BYTE]                  = mode;

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM711_PACKET_SIZE, CM_MM711_INTERRUPT_TIMEOUT);
}

void CMMM711Controller::SendSavePacket()
{
    unsigned char buffer[CM_MM711_PACKET_SIZE]  = { 0x00, 0x50, 0x55 };

    hid_write(dev, buffer, CM_MM711_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_MM711_PACKET_SIZE, CM_MM711_INTERRUPT_TIMEOUT);
}
