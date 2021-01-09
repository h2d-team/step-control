#include "generator.h"
#include "uart/controller.h"

bool motor::Generator::start(uint32_t A, uint32_t V, int32_t S)
{
    if (state_ != NONE)return false;
    if (S==0) return true;
    if (S<0){forward_ = false;S = -S;}
    else forward_ = true;

    v = v_ = n = n1_ = n2_ = 0;
    a_ = A;
    while (1)
    {
        v_ += a_;
        if (v_ <= V)
        {
            S -= 2*v_;
            if (S>=0) n1_++;
            else
            {
                S += 2*v_;
                v_ -= a_;
                break;
            }
        }
        else
        {
            v_ -= a_;
            break;
        }
    }

    if (n1_==0)
    {
        n1_ = 1;
        count_ = S % 2;
        if (count_==0) a_ = S/2;
        else a_ = S/2 + 1;
        r_ = 0;
        state_ = STAGE1;
    }
    else
    {
        n2_ = S/v_+1;
        S = (v_ - S%v_);
        r_ = S/n1_;
        count_ = S % n1_;
        state_ = STAGE1;
    }
    return true;
}

int32_t motor::Generator::generate()
{
    int32_t ret;
    switch (state_)
    {
    case STAGE1:
        if (n==0) v = a_ - r_;
        else v += a_;
        ret = v;
        if (n<count_) ret--;
        if (++n == n1_)
        {
            if (n2_ == 0) state_ = STAGE3;
            else state_ = STAGE2;
            n = 0;
        }
        break;

    case STAGE2:
        ret = v_;
        if (++n == n2_)
        {
            state_ = STAGE3;
            n = 0;
        }
        break;

    case STAGE3:
        if (n==0) v = n1_*a_;
        else v -= a_;
        ret = v;
        if (++n == n1_) state_ = NONE;
        break;
    default:
        ret = 0;
        break;
    }
    if (forward_) return ret;
    else return -ret;
}
