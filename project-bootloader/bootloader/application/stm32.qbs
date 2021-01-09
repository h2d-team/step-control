import qbs

Product
{
    name: "application"
    Properties
    {
        condition: qbs.buildVariant === "debug"
        type: ["application", "hex"] //for build
    }

    Properties
    {
        condition: qbs.buildVariant === "release"
        type: ["application", "hex", "flash"] //for deploy to board
    }
    Depends{name: "cpp"}
    Depends{name: "stm32f1"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "bootloader.cpp",
        "bootloader.h",
        "config.h",
        "controller.cpp",
        "controller.h",
        "main.cpp",
        "system_clock.cpp",
        "flash.cpp",
        "type.h",
    ]
}

