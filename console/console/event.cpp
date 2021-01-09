#include "manager.h"
#include "core/serial/controller.h"

void console::Manager::serialReceived_(quint8 length, quint16 type, quint8* data)
{
    switch (type)
    {
    case console::MessageReceived:
    {
        QString msg((char*)data);
        console::Manager::instance()->showMessage(msg);
        break;
    }
    case console::PlotReceived:
    {
        console::Manager::instance()->displayPlot_(data, length);
        break;
    }
    case console::FieldReceived:
    {
        uint16_t cmd = ((uint16_t)data[0] << 8) | ((uint16_t)data[1]);
        int32_t val = ((int32_t)data[2] << 24) | ((int32_t)data[3] << 16) | ((int32_t)data[4] << 8) | ((int32_t)data[5]);
        console::Manager::instance()->fieldUpdated_(cmd, val);
        break;
    }

    case console::boot_start:
    {
        crc_Generate((uint8_t *) lines[lines_index].toStdString().c_str(), lines[lines_index].length());
        serial::Controller::instance()->send(console::boot_download, lines[lines_index].length(), (uint8_t *) lines[lines_index].toStdString().c_str());
        break;
    }
    case console::boot_download:
    {
        if(++lines_index < lines.count())
        {
            crc_Generate((uint8_t *) lines[lines_index].toStdString().c_str(), lines[lines_index].length());
            serial::Controller::instance()->send(console::boot_download, lines[lines_index].length(), (uint8_t *) lines[lines_index].toStdString().c_str());
        }
        else
        {
            uint8_t buf[4];
            uint32_t crc = crc_Get();
            qDebug()<<"crc "<<crc;

            buf[0] = (crc >> 24 ) & 0xFF;
            buf[1] = (crc >> 16 ) & 0xFF;
            buf[2] = (crc >> 8 ) & 0xFF;
            buf[3] = (crc & 0xFF);

            serial::Controller::instance()->send(console::boot_end, 4, buf);
        }

        fieldUpdated_((uint16_t)console::Field_::PercentDownload, (lines_index * 100)/lines.count());

        break;
    }
    case console::boot_end: //check crc
    {
        if(length > 0)
        {
            if(0 == data[0])
            {
                serial::Controller::instance()->send(console::boot_app_mode, 0, nullptr);
                fieldUpdated_((uint16_t)console::Field_::BootModeEnable, 0u);
                console::Manager::instance()->showMessage("Download firmware complete, wait to jump to app mode");
            }
        }        

        break;

    }

    case console::boot_bootloader_mode_require: //require boot mode or no
    {
        if(requestJumBootLoader)
        {
            serial::Controller::instance()->send(console::boot_bootloader_mode, 0, nullptr);
        }
        break;
    }

    case console::boot_bootloader_mode: //jump boot mode success
    {
        if(requestJumBootLoader)
            requestJumBootLoader = false;
        fieldUpdated_((uint16_t)console::Field_::BootModeEnable, 1u);
        break;
    }

        /*TODO: others*/
    }
}

void console::Manager::fieldUpdated_(uint16_t type, int32_t val)
{
    emit fieldUpdated(type, val);
}
