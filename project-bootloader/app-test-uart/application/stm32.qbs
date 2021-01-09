import qbs

Product
{
    name: "application"
    Properties
    {
        condition: qbs.buildVariant === "debug"
        type: ["application", "hex", "flash"] //for deploy to board
    }

    Properties
    {
        condition: qbs.buildVariant === "release"
        type: ["application", "hex"] //for build
    }
    Depends{name: "cpp"}
    Depends{name: "stm32f1"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "main.cpp",
    ]
}

