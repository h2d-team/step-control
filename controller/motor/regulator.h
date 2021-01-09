#ifndef MOTOR_REGULATE_H
#define MOTOR_REGULATE_H
#include "controller.h"
#include "hal/hal.h"

inline M_EVENT_HANDLER(motor::Controller,regulate)
{
    int32_t p = (spiData_ & 0x3FFF) - angleOffset_;
    if(p < 0) p += 0x4000;
    if ((++cycleCount_ % 5) == 0)
    {
        this->updatePosition_(p); //5kHz:5 = 1kHz
        cycleCount_ = 0;
    }

    /* calculate sinTheta, cosTheta*/
    uint16_t theta = ((p*25)>>2) % (PULSE_PI_2*4); /* 200*(pi/2) = 200*PULSE_PI_2 = 200 * 512 ~ 2^14 (1 rev) => 25 * 2^12 ~ 2^14 => 25 value in LUT ~ 4 pulse encoder*/
    int32_t sinTheta, cosTheta;
    if (theta < PULSE_PI_2)
    {
        sinTheta = SINCOS[theta];
        cosTheta = SINCOS[PULSE_PI_2 - theta];
    }
    else if (theta < (PULSE_PI_2*2))
    {
        sinTheta = SINCOS[(PULSE_PI_2*2) - theta];
        cosTheta = -SINCOS[theta - PULSE_PI_2];
    }
    else if (theta < (PULSE_PI_2*3))
    {
        sinTheta = -SINCOS[theta - (PULSE_PI_2*2)];
        cosTheta = -SINCOS[(PULSE_PI_2*3) - theta];
    }
    else if (theta < (PULSE_PI_2*4))
    {
        sinTheta = -SINCOS[(PULSE_PI_2*4) - theta];
        cosTheta = SINCOS[theta - (PULSE_PI_2*3)];
    }

    /* Park transformation */
    int32_t id = ia_ * cosTheta + ib_ * sinTheta;       //Q12.16 = Q12.4 * Q0.12
    int32_t iq = ib_ * cosTheta - ia_ * sinTheta;       //Q12.16

    /* id PI regulators */
    int32_t ed = (0 - id) >> 12;                        //Q12.4
    dSum_ += dKI_*ed;                                   //Q12.14
    if (dSum_ > MAX_DSUM) dSum_ = MAX_DSUM;
    else if (dSum_ < -MAX_DSUM) dSum_ = -MAX_DSUM;
    int32_t vD = (dKP_*ed + dSum_) >> 10;               //Q12.4
    if (vD > MAX_VD) vD = MAX_VD;
    else if (vD < -MAX_VD) vD = -MAX_VD;

    /* iq PI regulators */
    int32_t eq = (iqRef_ - iq) >> 12;                   //Q12.4
    qSum_ += qKI_*eq;
    if (qSum_ > MAX_QSUM) qSum_ = MAX_QSUM;
    else if (qSum_ < -MAX_QSUM) qSum_ = -MAX_QSUM;
    int32_t vQ = (qKP_*eq + qSum_) >> 10;               //Q12.4
    if (vQ > MAX_VQ) vQ = MAX_VQ;
    else if (vQ < -MAX_VQ) vQ = -MAX_VQ;

    /* inverse Park transformation */
    int32_t vA = (vD * cosTheta - vQ * sinTheta);       //Q12.16
    int32_t vB = (vD * sinTheta + vQ * cosTheta);       //Q12.16

    vA >>= 15;                                              //Q11.0
    vB >>= 15;                                              //Q11.0

    if (vA > MAX_DUTY) vA = MAX_DUTY;
    else if (vA < - MAX_DUTY) vA = -MAX_DUTY;

    if (vB > MAX_DUTY) vB = MAX_DUTY;
    else if (vB < -MAX_DUTY) vB = - MAX_DUTY;

    uint16_t ah, al, bh, bl;
    if (vA > 0){ah = vA; al = 0;}
    else {ah = 0; al = -vA;}
    if (vB > 0){bh = vB; bl = 0;}
    else {bh = 0; bl = -vB;}

    if (motorEnable_)
    {
        SET_DUTY(ah, al, bh, bl);
    }
    else
    {
        SET_DUTY(0,0,0,0);
    }
}

#define MAX_PSUM (1024LL<<10)
#define MAX_VSUM (1024LL<<22)
#define MAX_IQREF (1024LL<<20) /*Q15.16*/
#define IQ_SHIFT 11

/*output: iqRef_ (Q15.16)*/
inline void motor::Controller::updatePosition_(int32_t p) //1kHz
{
    /*position and speed estimation*/
    int16_t delta = p - angle_;
    if (delta > MAX_POSITION/2) delta -= MAX_POSITION;
    else if (delta < -MAX_POSITION/2) delta += MAX_POSITION;
    p_ += delta; //Q14.0
    v_ -= vFilter_[vFilterIndex_]; //Q14.4
    v_ += delta;
    vFilter_[vFilterIndex_] = delta;
    vFilterIndex_++;
    if (vFilterIndex_ >= SPEED_W_SIZE) vFilterIndex_ = 0;
    angle_ = p;

    /*profile generation*/
    int32_t nextVel = generator_.generate();

    aRef_ = nextVel - vRef_;
    vRef_ = nextVel;
    pRef_ += vRef_;

    /*position regulation*/
    int32_t ep = pRef_ - (p_<<POSITION_SHIFT);
    pSum_ += pKI_*ep;
    if (pSum_ > MAX_PSUM) pSum_ = MAX_PSUM;
    else if (pSum_ < -MAX_PSUM) pSum_ = -MAX_PSUM;

    int64_t vCmd = pSum_ + pKP_ * ep + pKD_*(ep - lastE_); //PID
    lastE_ = ep;

    /*speed regulation*/
    int32_t ev = vCmd + KVff_* vRef_ - v_;
    vSum_ += vKI_*ev;
    if (vSum_ > MAX_VSUM) vSum_ = MAX_VSUM;
    else if (vSum_ < -MAX_VSUM) vSum_ = -MAX_VSUM;

    int64_t ret = vSum_ + vKP_*ev + KAff_*aRef_; //PI
    if (ret > MAX_IQREF) iqRef_ = MAX_IQREF;
    else if (ret < -MAX_IQREF) iqRef_ = -MAX_IQREF;
    else   iqRef_ = ret;
}

#endif // REGULATE_H
