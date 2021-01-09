#include "uart/controller.h"
#include "controller.h"
#include "bootloader.h"

int main()
{
    /* Init */
    uint32_t value = 0u;
    application::CONTROLLER.init();
    uart::CONTROLLER.init();

    application::CONTROLLER.readF_(FLASH_BEGIN_ADDRESS, &value, sizeof(value));
    if(APP_MODE_VALUE == value)
    {
        /*If user app does not exist*/
        application::CONTROLLER.readF_(FLASH_BEGIN_ADDRESS, &value, sizeof(value));
        if(0xFFFFFFFFu == value)
        {
            uart::CONTROLLER.printMessage("No user app.");
        }
        /*If user app is available*/
        else
        {
            uart::CONTROLLER.sendMessage(Command::boot_bootloader_mode_require, 0, nullptr); //Request boot_mode
            uint32_t loopIndex = 0;
            while (++loopIndex < 0x10001Fu)
            {
                //Wait request boot mode from PC.
                uart::CONTROLLER.loop();
            }

            bootloader::CONTROLLER.boot_jump_to_app(APP_BEGIN_ADDRESS);
            uart::CONTROLLER.printMessage("Boot fail...");

        }

    }
    else
    {
        uart::CONTROLLER.sendMessage(Command::boot_bootloader_mode, 0, nullptr); //boot_mode
        uart::CONTROLLER.printMessage("Bootloader mode.");
    }

    while (true)
    {
        uart::CONTROLLER.loop();
    }

    return 0;
}
