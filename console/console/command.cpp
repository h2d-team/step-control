#include "manager.h"
#include "core/serial/controller.h"

void console::Manager::move(qint32 s)
{
    uint8_t data[4];

    data[0] = ((s >> 24) & 0xFF);
    data[1] = ((s >> 16) & 0xFF);
    data[2] = ((s >> 8) & 0xFF);
    data[3] = (s & 0xFF);

    qDebug()<<"move to:"<<s;

    serial::Controller::instance()->send(console::MoveMotor, 4, data);
}

void console::Manager::flashFields()
{
    //serial::Controller::instance()->send(console::FLASH_FIELDS, 0, nullptr);
}

void console::Manager::calibrateMotor()
{
    serial::Controller::instance()->send(console::CalibrateMotor, 0, nullptr);
}

void console::Manager::startMotor()
{
    serial::Controller::instance()->send(console::StartMotor, 0, nullptr);
}
void console::Manager::stopMotor()
{    
    serial::Controller::instance()->send(console::StopMotor, 0, nullptr);
}

void console::Manager::updateFirmware()
{
    lines_index = 0;
    lines.clear();
    QUrl url(fileName);
    QFile inputFile(url.toLocalFile());
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.length())
          {
              lines.append(line);
          }
       }
       inputFile.close();
    }
    if (lines.count())
    {
        crc_ResetSeed();
        serial::Controller::instance()->send(console::boot_start, 0, nullptr);
    }
    else
        console::Manager::instance()->showMessage("File empty");
}

void console::Manager::loadFields()
{
    qDebug()<<"loadFields";
    serial::Controller::instance()->send(console::SyncFields, 0, nullptr);
}

void console::Manager::updateField(quint16 type, QString v)
{
    bool ok = false;
    int32_t val = v.toInt(&ok);

    if (ok)
    {
        uint8_t buf[6];

        buf[0] = (type >> 8 ) & 0xFF;
        buf[1] = (type & 0xFF);

        buf[2] = (val >> 24 ) & 0xFF;
        buf[3] = (val >> 16 ) & 0xFF;
        buf[4] = (val >> 8 ) & 0xFF;
        buf[5] = (val & 0xFF);
        serial::Controller::instance()->send(console::UpdateField, 6, buf);
    }
    else
    {
        this->showMessage("Invalid value:" + v);
    }
}


void console::Manager::crc_CreateLookUpTable(void)
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

void console::Manager::crc_ResetSeed(void)
{
    crc_Value = 0u;
}

void console::Manager::crc_Generate(const uint8_t * const buff, const uint16_t length)
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

uint32_t console::Manager::crc_Get(void)
{
    return crc_Value;
}

