/*---------------------------------------------------------*\
| OpenRGBPhilipsHueSettingsEntry.cpp                        |
|                                                           |
|   User interface for OpenRGB Philips Hue settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPhilipsHueSettingsEntry.h"
#include "ui_OpenRGBPhilipsHueSettingsEntry.h"

using namespace Ui;

OpenRGBPhilipsHueSettingsEntry::OpenRGBPhilipsHueSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPhilipsHueSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBPhilipsHueSettingsEntry::~OpenRGBPhilipsHueSettingsEntry()
{
    delete ui;
}

void OpenRGBPhilipsHueSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBPhilipsHueSettingsEntry::on_UnpairButton_clicked()
{
    ui->UsernameValue->setText("");
    ui->ClientKeyValue->setText("");
}
