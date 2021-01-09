import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import "../commons"

ToolBar
{
    id: serialSection
    width: parent.width
    //background: Rectangle{color: '#444'}
    Material.primary: Material.Teal
    Material.accent: Material.Pink
    signal clearLog()
    RowLayout
    {
        id: container
        width: parent.width - 10
        anchors.centerIn: parent
        Button
        {
            id: btnClean
            height: 45
            Layout.preferredWidth: height
            text: Flaticon.get('brush1')
            font.family: 'Flaticon'
            font.pixelSize: 15
            onClicked: clearLog()
            Material.background: Material.Indigo
        }

        Button
        {
            Layout.fillWidth: true
            text: SerialPort.bandwidth + ' bytes/s'
            visible: !btnConnect.visible
            Material.background: Material.Green
            font.bold: true
            font.pixelSize: 18
        }

//        Rectangle
//        {
//            border.width: 1
//            border.color: '#ccc'
//            Layout.fillWidth: true
//            Layout.preferredHeight: 40
//            color: "transparent"
//            radius: 3
//            Label
//            {
//                anchors.centerIn: parent
//                text: SerialPort.bandwidth + ' B/s'
//                //text: '1234 B/s'
//                color: '#eee'
//                font.pixelSize: 15
//                font.bold: true
//            }
//            visible: !btnConnect.visible
//        }

        Button
        {
            id: btPause
            //Material.foreground: '#444'
            text: 'Pause';
            Material.background: Material.Indigo
            onClicked:
            {
                if (text === 'Pause')
                {
                    Console.pause(true);
                    text ='Resume'
                }
                else
                {
                    Console.pause(false);
                    text ='Pause'
                }
            }
            visible: !btnConnect.visible
        }

        ComboBox
        {
            id: port
            Layout.fillWidth: true
            model: SerialPort.ports()
            visible: btnConnect.visible
        }
        Button
        {
            id: btnRefresh
            Layout.preferredWidth: height
            Material.foreground: '#444'
            text: Flaticon.get('restart')
            font.family: 'Flaticon'
            onClicked: port.model = SerialPort.ports()
            visible: btnConnect.visible
        }
        Button
        {
            id: btnConnect
            //Material.foreground: Material.Green
            text: 'Connect'
            visible: true
            highlighted: true
            onClicked:
            {
                if (SerialPort.open(port.currentText))
                {
                    btnConnect.visible = false
                    Console.loadFields() //READ DATA
                    Console.showMessage("SerialPort Connected")
                }
                else
                {
                    Console.showMessage("Can not connect to SerialPort")
                }
            }
        }
        Button
        {
            id: btnDisconnect
            text: 'Disconnect'
            visible: !btnConnect.visible
            Material.background: Material.Indigo
            onClicked:
            {
                SerialPort.close();
                btnConnect.visible = true
                Console.showMessage("SerialPort Disconnected")
            }
        }
    }
}
