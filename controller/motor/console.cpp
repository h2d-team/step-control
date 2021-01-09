#include "controller.h"
#include "type.h"

M_EVENT_HANDLER(motor::Controller,plot)
{
    int index = 0;
    int32_t pref = (pRef_>>POSITION_SHIFT);
    int32_t p = p_;

    int32_t vref = vRef_;
    int32_t v = v_;

//    int32_t vref = ia_;//vRef_;
//    int32_t v = ib_;//v_;

    uint8_t data[12];
    data[index++] = ((pref >> 24) & 0xFF);
    data[index++] = ((pref >> 16) & 0xFF);
    data[index++] = ((pref >> 8) & 0xFF);
    data[index++] = (pref & 0xFF);

    data[index++] = ((p >> 24) & 0xFF);
    data[index++] = ((p >> 16) & 0xFF);
    data[index++] = ((p >> 8) & 0xFF);
    data[index++] = (p & 0xFF);

    data[index++] = ((vref >> 8) & 0xFF);
    data[index++] = (vref & 0xFF);

    data[index++] = ((v >> 8) & 0xFF);
    data[index++] = (v & 0xFF);
    uart::Controller::instance().sendPacket(console::PlotReceived, 12,data);
}

U_COMMAND_HANDLER(motor::Controller, updateField)
{
    uart::Controller::instance().print("Receive UpdateField Command");

    uint16_t cmd = ((uint16_t)data[0] << 8) | ((uint16_t)data[1]);
    int32_t val = ((int32_t)data[2] << 24) | ((int32_t)data[3] << 16) | ((int32_t)data[4] << 8) | ((int32_t)data[5]);

    switch (cmd)
    {
        case console::PKP: pKP_ = val; break;
        case console::PKI: pKI_ = val; break;
        case console::PKD: pKD_ = val; break;
        case console::VKP: vKP_ = val; break;
        case console::VKI: vKI_ = val; break;
        case console::KAFF: KAff_ = val; break;
        case console::KVFF: KVff_ = val; break;
        case console::DKP: dKP_ = val; break;
        case console::DKI: dKI_ = val; break;
        case console::QKP: qKP_ = val; break;
        case console::QKI: qKI_ = val; break;
        case console::A: A_ = val; break;
        case console::V: V_ = val; break;
        default:
            uart::Controller::instance().print("Field type error");
            break;
    }
    fieldUpdated_(cmd, val);
}

void motor::Controller::fieldUpdated_(uint16_t cmd, int32_t val)
{
    uint8_t buf[6];
    buf[0] = ((cmd>>8) & 0xFF);
    buf[1] = (cmd & 0xFF);
    buf[2] = ((val>>24) & 0xFF);
    buf[3] = ((val>>16) & 0xFF);
    buf[4] = ((val>>8) & 0xFF);
    buf[5] = (val & 0xFF);
    uart::Controller::instance().sendPacket(console::FieldReceived, 6, buf);
}

U_COMMAND_HANDLER(motor::Controller, syncFields)
{
    fieldUpdated_(console::PKP,pKP_);
    fieldUpdated_(console::PKI, pKI_);
    fieldUpdated_(console::PKD, pKD_);
    fieldUpdated_(console::VKP, vKP_);
    fieldUpdated_(console::VKI, vKI_);
    fieldUpdated_(console::KAFF, KAff_);
    fieldUpdated_(console::KVFF, KVff_);
    fieldUpdated_(console::DKP, dKP_);
    fieldUpdated_(console::DKI, dKI_);
    fieldUpdated_(console::QKP, qKP_);
    fieldUpdated_(console::QKI, qKI_);
    fieldUpdated_(console::A, A_);
    fieldUpdated_(console::V, V_);
}
