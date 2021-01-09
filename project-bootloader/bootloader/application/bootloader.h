#ifndef BOOTLOADER_H
#define BOOTLOADER_H


#include "stm32f103xb.h"
#include "type.h"
#include "config.h"

/** @brief Application area start address */
static uint32_t parser_StartAddress = APP_BEGIN_ADDRESS;

/** @brief Application area end address */
static uint32_t parser_EndAddress = APP_END_ADDRESS;

/** @brief Offset of address to support 24 or 32 bit addressing */
static uint32_t parser_AddressOffset = 0u;

/** @brief Counts, how often the data is invalid */
static uint8_t parser_InvalidDataCount = 0u;

typedef enum
{
    parserStatus_Success = 0x00u, /**< @brief Success */
    parserStatus_InvalidFormat = 0x01u, /**< @brief Invalid format */
    parserStatus_Undefined = 0xFF /**< @brief Undefined */
} parser_Status_t;

typedef struct
{
    uint32_t address; /**< @brief Start address */
    uint8_t data[PARSER_MAX_LENGTH]; /**< @brief Data */
    uint16_t length;/**< @brief Data length */
} parser_Data_t;

typedef void (*appCall)(void);

namespace bootloader
{

class Controller
{
public:
    Controller()
    {
        crc_CreateLookUpTable();
    }
    void boot_jump_to_app(uint32_t startAddress);
    parser_Status_t parser_ParseIntelHex(const uint8_t line[], parser_Data_t * const outData);

    /* Check crc */

    void crc_ResetSeed(void);
    void crc_Generate(const uint8_t * const buff, const uint16_t length);
    uint32_t crc_Get(void);

    uint32_t crc_LookUpTable[256u];


private:
    uint8_t parser_AsciiToHex(uint8_t data);
    uint8_t parser_TwoHexToByte(uint8_t const data[]);

    /* check crc */
    void crc_CreateLookUpTable();

    uint32_t crc_Value = 0u;

};

extern bootloader::Controller CONTROLLER;

}
#endif // BOOTLOADER_H
