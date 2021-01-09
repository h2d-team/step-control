import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import "../commons"
import console 1.0

Label
{
    Layout.fillWidth: true
    Layout.preferredHeight: 35
    background: Rectangle{color: '#ccc'}
    verticalAlignment: "AlignVCenter"
    font.bold: true
    leftPadding: 10
    font.capitalization: Font.AllUppercase
    font.pixelSize: 12
    color: Material.color(Material.Pink)
    //color: '#fff'
}
