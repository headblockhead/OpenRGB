#ifdef _WIN32

#include "AsusTUFLaptopController.h"
#include "RGBController_AsusTUFLaptopWMI.h"
#include "FanController_AsusTUFLaptopWMI.h"

#include "Detector.h"
#include "wmi.h"
#include <string>

static void DetectAsusTUFLaptopWMIControllers()
{
    // Try to retrieve ProductID / Device name from WMI; Possibly can be rewritten to use wmi.cpp
    // IF you encounter false detection ( e.g. if your laptop keyboard backlight uses USB interface
    // instead of ACPI WMI) please add a WHITELIST by checking the
    // `name` variable for model substrings like "FX505DU"
    // For now, checking for "TUF Gaming" should suffice

    Wmi wmi;

    std::vector<QueryObj> systemProduct;
    if (wmi.query("SELECT * FROM Win32_ComputerSystemProduct", systemProduct))
    {
        return;
    }

    // There should only be one, a cycle is a precaution
    if(systemProduct.size() != 1)
    {
        return;
    }
    std::string& name = systemProduct[0]["Name"];

    if(name.find("TUF Gaming") == name.npos)
    {
        return;
    }

    AsusTUFLaptopController* controller = AsusTUFLaptopController::checkAndCreate();
    if(controller)
    {
        RGBController* new_controller = new RGBController_AsusTUFLaptopWMI(controller);
        FanController* fan_controller = new FanController_AsusTUFLaptopWMI(controller);

        ResourceManager::get()->RegisterRGBController(new_controller);
        ResourceManager::get()->RegisterFanController(fan_controller);
    }
}   /* DetectAsusTUFLaptopWMIControllers() */

REGISTER_DETECTOR("ASUS TUF Laptop", DetectAsusTUFLaptopWMIControllers);

#endif
