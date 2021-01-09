import qbs

Project
{
    name: "test-uart"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "/home/dienpv/Downloads/gcc-arm-none-eabi-9-2020-q2-update/bin/"
    property string openOCDExe: "openocd"
    property string openOCDPath: "/usr/share/openocd"
    references:
    [
        "modules/stm32f1/mdk.qbs",
        /*LIBRARIES*/
        "uart/stm32.qbs",
        "application/stm32.qbs",
    ]
}
