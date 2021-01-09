import qbs

Product
{
    name: "motor"

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
        "controller_.h",
        "interrupt.cpp",
        "type.h",
        "regulator.h",
        "define.h",
        "calibrate.cpp",
        "console.cpp",
        "controller.cpp",
        "controller.h",
        "main.cpp",
        "generator.h",
        "generator.cpp",
    ]
}

