#include "bootloader.h"
bootloader::Controller bootloader::CONTROLLER;

/*Jump to user app from boot app*/
void bootloader::Controller::boot_jump_to_app(uint32_t startAddress)
{
    __disable_irq();
    /*Read SP value*/
    uint32_t msp = (uint32_t)(*((uint32_t *)startAddress));
    /*Read address of reset interrupt function of user app*/
    appCall app = (appCall)(*((uint32_t *)(startAddress + 4)));
    /*Update SCB_VTOR Value*/
    SCB->VTOR = startAddress;
    /*Update MSP Value*/
    __set_MSP(msp);
    /*Call reset interrupt function of user app*/
    app();
}

uint8_t bootloader::Controller::parser_AsciiToHex(uint8_t data)
{
    uint8_t retVal = 0xFFu;

    if ((data >= (uint8_t) '0') && (data <= (uint8_t) '9'))
    {
        retVal = (uint8_t) (data - (uint8_t) '0');
    }
    else if ((data >= (uint8_t) 'A') && (data <= (uint8_t) 'F'))
    {
        retVal = (uint8_t) (data + 10u - (uint8_t) 'A');
    }
    else
    {
        parser_InvalidDataCount++;
    }

    return retVal;
}


uint8_t bootloader::Controller::parser_TwoHexToByte(uint8_t const data[])
{
    uint8_t retVal = 0u;
    uint8_t highByte = 0u;
    uint8_t lowByte = 0u;

    highByte = parser_AsciiToHex(data[0u]);
    lowByte = parser_AsciiToHex(data[1u]);
    retVal = (uint8_t) (highByte << 4u | lowByte);

    return retVal;
}


parser_Status_t bootloader::Controller::parser_ParseIntelHex(const uint8_t line[],
        parser_Data_t * const outData)
{
    parser_Status_t status;
    uint8_t byteCount = 0u;
    uint32_t address = 0u;
    uint8_t recordType = 0u;
    uint8_t checksum = 0u;
    uint8_t dataTmp = 0u;
    uint16_t index = 0u;
    uint16_t offset = 3u;

    /* Reset output buffer */
    outData->address = 0u;
    outData->length = 0u;

    if ((uint8_t) ':' == line[0u])
    {
        /* Get byte count */
        byteCount = parser_TwoHexToByte((uint8_t const *) &line[1u]);
        /* Update checksum */
        checksum = (uint8_t) (checksum + byteCount);
        /* Get address */
        for (index = 0u; index < 2u; index++)
        {
            /* Convert tow hex digits to byte */
            dataTmp = parser_TwoHexToByte((uint8_t const *) &line[offset]);
            /* Update address */
            address = (uint32_t) (address << 8u | dataTmp);
            /* Update checksum */
            checksum = (uint8_t) (checksum + dataTmp);
            /* Update offset */
            offset = (uint8_t) (offset + 2u);
        }
        /* Get record type */
        recordType = parser_TwoHexToByte((uint8_t const *) &line[offset]);
        /* Update checksum */
        checksum = (uint8_t) (checksum + recordType);
        /* Update offset to point to data position */
        offset = (uint8_t) (offset + 2u);

        switch (recordType)
        {
            case 0u: /* Data */
                /* Update output address */
                outData->address = address + parser_AddressOffset;
                /* Update output data */
                for (index = 0u; index < byteCount; index++)
                {
                    /* Convert tow hex digits to byte */
                    dataTmp = parser_TwoHexToByte((uint8_t const *) &line[offset]);
                    /* Update data */
                    outData->data[index] = dataTmp;
                    /* Update checksum */
                    checksum = (uint8_t) (checksum + dataTmp);
                    /* Update offset */
                    offset = (uint8_t) (offset + 2u);
                }
                /* Update output data length */
                outData->length = byteCount;
                /* Verify checksum */
                checksum = (uint8_t) (checksum
                        + parser_TwoHexToByte((uint8_t const *) &line[offset]));
                if ((0u == checksum)
                        && ((outData->address >= parser_StartAddress)
                                && (outData->address
                                        <= (uint32_t) (parser_EndAddress - outData->length))
                            ))
                {
                    status = parserStatus_Success;
                }
                else
                {
                    status = parserStatus_InvalidFormat;
                }
                break;
            case 1u: /* End Of File */
                /* Verify checksum */
                checksum = (uint8_t) (checksum
                        + parser_TwoHexToByte((uint8_t const *) &line[offset]));
                if (0u == checksum)
                {
                    status = parserStatus_Success;
                }
                else
                {
                    status = parserStatus_InvalidFormat;
                }
                break;
            case 2u: /* Extended Segment Address */
                if (2u == byteCount)
                {
                    /* Get address offset */
                    for (index = 0u; index < byteCount; index++)
                    {
                        /* Convert tow hex digits to byte */
                        dataTmp = parser_TwoHexToByte((uint8_t const *) &line[offset]);
                        /* Update address offset */
                        parser_AddressOffset = (uint32_t) (parser_AddressOffset
                                << 8u | dataTmp);
                        /* Update checksum */
                        checksum = (uint8_t) (checksum + dataTmp);
                        /* Update offset */
                        offset = (uint8_t) (offset + 2u);
                    }
                    /* Update address offset */
                    parser_AddressOffset *= 16u;
                    /* Verify checksum */
                    checksum = (uint8_t) (checksum
                            + parser_TwoHexToByte((uint8_t const *) &line[offset]));
                    if (0u == checksum)
                    {
                        status = parserStatus_Success;
                    }
                    else
                    {
                        status = parserStatus_InvalidFormat;
                    }
                }
                else
                {
                    status = parserStatus_InvalidFormat;
                }
                break;
            case 3u: /* Start Segment Address */
                /* Record type 3 not supported */
                status = parserStatus_Success;
                break;
            case 4u: /* Extended Linear Address */
                if (2u == byteCount)
                {
                    /* Get address offset */
                    for (index = 0u; index < byteCount; index++)
                    {
                        /* Convert tow hex digits to byte */
                        dataTmp = parser_TwoHexToByte((uint8_t const *) &line[offset]);
                        /* Update address offset */
                        parser_AddressOffset = (uint32_t) (parser_AddressOffset
                                << 8u | dataTmp);
                        /* Update checksum */
                        checksum = (uint8_t) (checksum + dataTmp);
                        /* Update offset */
                        offset = (uint8_t) (offset + 2u);
                    }
                    /* Update address offset */
                    parser_AddressOffset =
                            (uint32_t) (parser_AddressOffset << 16u);
                    /* Verify checksum */
                    checksum = (uint8_t) (checksum
                            + parser_TwoHexToByte((uint8_t const *) &line[offset]));
                    if (0u == checksum)
                    {
                        status = parserStatus_Success;
                    }
                    else
                    {
                        status = parserStatus_InvalidFormat;
                    }
                }
                else
                {
                    status = parserStatus_InvalidFormat;
                }
                break;
            case 5u: /* Start Linear Address */
                /* Record type 5 not supported */
                status = parserStatus_Success;
                break;
            default:
                status = parserStatus_InvalidFormat;
                break;
        }
    }
    else
    {
        status = parserStatus_InvalidFormat;
    }

    if (0u != parser_InvalidDataCount)
    {
        status = parserStatus_InvalidFormat;
    }

    return status;
}


void bootloader::Controller::crc_CreateLookUpTable(void)
{
    uint16_t crcIndex = 0u;
    uint32_t crcVal = 0u;
    uint8_t bitWise = 0u;

    for (crcIndex = 0u; crcIndex < 256u; crcIndex++)
    {
        crcVal = crcIndex;
        for (bitWise = 0u; bitWise < 8u; bitWise++)
        {
            /* Top bit is 0x00000001 for reserved polynomial */
            if (crcVal & 0x01u)
            {
                /* Fixed CRC polynomial 0xEDB88320 */
                crcVal = (crcVal >> 1u) ^ 0xEDB88320u;
            }
            else
            {
                crcVal = (crcVal >> 1u);
            }
        }
        /* Update CRC value to lookup table */
        crc_LookUpTable[crcIndex] = crcVal;
    }
}

void bootloader::Controller::crc_ResetSeed(void)
{
    crc_Value = 0u;
}

void bootloader::Controller::crc_Generate(const uint8_t * const buff, const uint16_t length)
{
    uint8_t crcDataTmp = 0u;
    uint16_t dataIndex = 0u;

    for (dataIndex = 0u; dataIndex < length; dataIndex++)
    {
        /* XORs the calculating CRC with input data, gets the significant low byte */
        crcDataTmp = (uint8_t)((crc_Value & 0xFFu) ^ buff[dataIndex]);
        /* Calculates CRC for each byte of data by looking into CRC lookup table */
        crc_Value = crc_LookUpTable[crcDataTmp] ^ (crc_Value >> 8u);
    }
}

uint32_t bootloader::Controller::crc_Get(void)
{
    return crc_Value;
}
