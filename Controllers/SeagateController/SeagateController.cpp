/*-----------------------------------------*\
|  SeagateController.cpp                    |
|                                           |
|  Code for Seagate Firecuda External HDD   |
|  RGB controller                           |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2023   |
\*-----------------------------------------*/

#include "SeagateController.h"

#include "WinIoCtl.h"
#include "ntddscsi.h"

SeagateController::SeagateController(HANDLE fd, wchar_t* path)
{
    this->fd    = fd;
	this->path  = path;
}

SeagateController::~SeagateController()
{

}

std::string SeagateController::GetLocation()
{
	std::string str(path.begin(), path.end());
    return("SCSI: " + str);
}

void SeagateController::SetLED
    (
    unsigned char led_id,
    unsigned char r,
    unsigned char g,
    unsigned char b
    )
{
    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold SCSI_PASS_THROUGH_DIRECT                                |
    | Size must be enough for the SCSI_PASS_THROUGH_DIRECT struct plus the sense    |
    | data.  Size of 80 bytes taken from captured data                              |
    \*-----------------------------------------------------------------------------*/
    unsigned char buffer[80]                = {0};

    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold RGB control data                                        |
    \*-----------------------------------------------------------------------------*/
    unsigned char data[14]                  = {0};
    data[0]                                 = 0x0E;
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = 0x01;
    data[8]                                 = 0x03;
    data[9]                                 = r;
    data[10]                                = g;
    data[11]                                = b;
    data[12]                                = 0x00;
    data[13]                                = 0xFF;

    /*-----------------------------------------------------------------------------*\
    | Create PSCSI_PASS_THROUGH_DIRECT pointer and point it to the buffer           |
    \*-----------------------------------------------------------------------------*/
    PSCSI_PASS_THROUGH_DIRECT command       = (PSCSI_PASS_THROUGH_DIRECT)buffer;

    /*-----------------------------------------------------------------------------*\
    | Set up pass through command                                                   |
    \*-----------------------------------------------------------------------------*/
    command->Length                         = sizeof(SCSI_PASS_THROUGH_DIRECT);
    command->ScsiStatus                     = 0x00;
    command->PathId                         = 0x00;
    command->TargetId                       = 0x00;
    command->Lun                            = 0x00;
    command->CdbLength                      = 0x0C;
    command->SenseInfoLength                = 0x20;
    command->DataIn                         = SCSI_IOCTL_DATA_OUT;
    command->DataTransferLength             = 0x0000000E;
    command->TimeOutValue                   = 0x00000014;
    command->DataBuffer                     = &data;
    command->SenseInfoOffset                = 0x0000002E;

    command->Cdb[0]                         = 0xD2;
    command->Cdb[1]                         = 0x53;
    command->Cdb[2]                         = 0x65;
    command->Cdb[3]                         = 0x74;
    command->Cdb[4]                         = 0x4C;
    command->Cdb[5]                         = 0x65;
    command->Cdb[6]                         = 0x64;
    command->Cdb[7]                         = 0x00;
    command->Cdb[8]                         = 0x00;
    command->Cdb[9]                         = 0x30;
    command->Cdb[10]                        = 0x0E;
    command->Cdb[11]                        = 0x00;

    /*-----------------------------------------------------------------------------*\
    | Send pass through command                                                     |
    \*-----------------------------------------------------------------------------*/
    DeviceIoControl(fd, IOCTL_SCSI_PASS_THROUGH_DIRECT, command, 80, command, 80, NULL, NULL);
}