import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window
{
    visible: true
    title: qsTr("Step Console")

    Component.onCompleted:
    {
        showView('qrc:/home/StartupView.qml')
    }

    FontLoader
    {
        source: 'qrc:/font/Flaticon.ttf'
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0
        Rectangle
        {
            id: errorMessage
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: '#E91E63'
            Label
            {
                id: txtError
                anchors.centerIn: parent
                text: 'Connecting to internet...'
                font.pixelSize: 14
                font.bold: true
                color: '#eee'
            }
            visible: false

            Timer
            {
                id: errorTimer
                interval: 500; running: false; repeat: false
                onTriggered: errorMessage.visible = false;
            }
        }

        StackView
        {
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: pageLoader
        }
    }

    function showAppError(message, timeout)
    {
        txtError.text = message
        errorMessage.visible = true;
        if (timeout>0)
        {
            errorTimer.interval = timeout
            errorTimer.start()
        }
    }

    Timer
    {
        id: timeOut
        interval: 15000; running: false; repeat: false
        onTriggered:
        {
            showAppError("The connection to the server failed.", 5000)
            hideLoading();
        }
    }

    function showView(item)
    {
        pageLoader.clear()
        pageLoader.push(item)
    }
}
