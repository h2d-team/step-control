#include "controller.h"
#include "uart/controller.h"
#include "bootloader.h"

application::Controller application::CONTROLLER;

static parser_Data_t outData_ = {};
static int error = 0;
void uart::Controller::processCommand_()
{
    switch (rxType_)
    {
        case Command::boot_start:
        {
            /* eraseSector page 8 to 62 */
            for (int i = 8; i < 63; i++)
                application::CONTROLLER.eraseSector_(BASE_ADDRESS + (i * (0x0400)));

            uart::CONTROLLER.sendMessage(Command::boot_start, 0, nullptr);
            uart::CONTROLLER.printMessage("EraseSector Success");

            //reset crc
            bootloader::CONTROLLER.crc_ResetSeed();
            break;
        }
        case Command::boot_download:
        {
            bootloader::CONTROLLER.crc_Generate(rxBuffer_,rxLength_);
            parser_Status_t st = bootloader::CONTROLLER.parser_ParseIntelHex(rxBuffer_,&outData_);
            if(st != parserStatus_Success)
            {
                __disable_irq();
                application::CONTROLLER.eraseSector_(BASE_ADDRESS + (8*0x0400));
                __enable_irq();
                uart::CONTROLLER.printfMessage("boot_download fail code(%d)", st);
                break;
            }
            if(0 != outData_.length)
            {
                __disable_irq();
                /*Write data to address in flash*/
                if(0 != (outData_.length % 2)){
                   outData_.length += 1;
                }
                error = application::CONTROLLER.writeF_(outData_.address, outData_.data, outData_.length);
                __enable_irq();

                if(error)
                {
                    uart::CONTROLLER.printfMessage("Write boot download error(%d)",error);
                    break;
                }
            }

            uart::CONTROLLER.sendMessage(Command::boot_download, 0, nullptr);
            break;
        }

        case Command::boot_end: //check CRC
        {
            uint8_t crc[1] = {0};
            uint32_t val = UINT32_DECODE_VAL;

            if(val != bootloader::CONTROLLER.crc_Get())
            {
                crc[0] = 0xFF;
                uart::CONTROLLER.printfMessage("Check crc error");
            }

            uart::CONTROLLER.sendMessage(Command::boot_end, 1, crc);
            break;
        }
        case Command::boot_bootloader_mode: //Vao bootmode
        {
             __disable_irq();
             application::CONTROLLER.eraseSector_(FLASH_BEGIN_ADDRESS);
              __enable_irq();
              NVIC_SystemReset(); //reset
             break;
        }
        case Command::boot_app_mode: //Vao app mode
        {
            uint32_t value = APP_MODE_VALUE;
            __disable_irq();
            application::CONTROLLER.eraseSector_(FLASH_BEGIN_ADDRESS);
            error = application::CONTROLLER.writeF_(FLASH_BEGIN_ADDRESS, &value, sizeof(value));
             __enable_irq();
            if(error)
            {
                uart::CONTROLLER.printfMessage("Write app mode error(%d)",error);
            }
            else
            {

                NVIC_SystemReset(); //reset
            }
             break;
        }


    default: break;
    }
}
