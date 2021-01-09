import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import "../commons"
import console 1.0

RowLayout
{
    id: fieldRow
    property alias name: txtName.text
    property alias text: txtDistance.text
    signal accepted()

    Layout.fillWidth: true
    Layout.preferredHeight: 35
    spacing: 0
    Label
    {
        id: txtName
        Layout.fillWidth: true
        Layout.fillHeight: true
        verticalAlignment: "AlignVCenter"
        //horizontalAlignment: "AlignRight"
        text:'A'
        font.bold: true
        leftPadding: 10
        //background: Rectangle{color: '#ddd'}
        font.pixelSize: 15
        color: '#444'
    }
//    Rectangle
//    {
//        Layout.fillHeight: true
//        Layout.preferredWidth: 1
//        color: '#ccc'
//        anchors.bottom: parent.bottom
//    }

//    Label
//    {
//        id: txtValue
//        Layout.preferredWidth: 80
//        Layout.fillHeight: true
//        verticalAlignment: "AlignVCenter"
//        horizontalAlignment: "AlignRight"
//        text:'undefined'
//        rightPadding: 8
//        leftPadding: 8
//        font.pixelSize: 15
//    }
    Rectangle
    {
        Layout.fillHeight: true
        Layout.preferredWidth: 1
        color: '#ccc'
    }
    TextField
    {
        id: txtDistance
        Layout.preferredWidth: 90
        Layout.fillHeight: true
        horizontalAlignment: "AlignRight"
        bottomPadding: 8
        background: Rectangle{color: 'transparent'}
        leftPadding: 8
        rightPadding: 10
        selectByMouse: true
        validator: IntValidator {bottom: -1000000; top: 1000000}
        text:'0'
        onFocusChanged:
        {
            if (activeFocus) selectAll()
        }
        font.pixelSize: 15
        onAccepted: fieldRow.accepted()
    }
}
