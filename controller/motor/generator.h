#ifndef BLDC_CONTROLLER_H
#define BLDC_CONTROLLER_H
#include <stdint.h>

namespace motor
{

class Generator
{
public:
    enum State{NONE=0, STAGE1, STAGE2, STAGE3, ZERO};
    bool start(uint32_t a, uint32_t v, int32_t s);
    int32_t generate();
    void reset(){state_ = NONE;}
    bool moving(){return state_ != NONE;}
private:
    uint32_t v, n;
    uint32_t a_, v_;
    uint32_t n1_, n2_;
    uint32_t r_,count_;
    bool forward_;
    int state_ = NONE;
};

}

#endif // BLDC_CONTROLLER_H
