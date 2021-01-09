import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import QtCharts 2.3
import "../commons"

Rectangle
{
    anchors.fill: parent

    Timer
    {
        id: timer
        interval: 1000; running: true; repeat: false
        onTriggered: showView('qrc:/home/HomeView.qml')
    }

    Column
    {
        anchors.centerIn: parent
        spacing: 5
        Label
        {
            text: "Welcome Step Console"
            font.pixelSize: 32
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label
        {
            id: loading
            text: "Starting up ..."
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
