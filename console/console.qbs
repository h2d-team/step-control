import qbs

Project
{
    name: "console"
    minimumQbsVersion: "1.7.1"

    property string protocPath: "/usr/local/bin/protoc"
    property string protoIncludePath: "/usr/local/include"
    property string protoLibPath: "/usr/local/lib"
    property bool useQt: true

    references:
    [
        "core/core.qbs",
        "flaticon/flaticon.qbs",
        "console/console.qbs",
        "ui/ui.qbs",
    ]
    qbsSearchPaths: ["../commons/"]
}

