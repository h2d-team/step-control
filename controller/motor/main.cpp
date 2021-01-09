#include "hal/controller.h"
#include "uart/controller.h"
#include "controller.h"

int main()
{
    COMPONENT_REG(hal,Controller);
    COMPONENT_REG(uart,Controller);
    COMPONENT_REG(motor,Controller);
    while (1)
    {
        core::Engine::instance().run();
    }

    return 0;
}
