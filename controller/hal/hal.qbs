import qbs 1.0

StaticLibrary
{
    name: "hal"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    Depends { name:"mdk"}
    Depends { name:"core"}

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:[product.sourceDirectory]
    }

    cpp.includePaths:
    [
        project.sourceDirectory,
    ]

    cpp.cxxLanguageVersion: "c++11"

    files:
    [
        "adc.cpp",
        "clock.cpp",
        "controller.h",
        "drv8841.cpp",
        "hal.h",
        "pwm.cpp",
        "spi.cpp",
        "tick.cpp",
        "uart.cpp",
    ]
}


