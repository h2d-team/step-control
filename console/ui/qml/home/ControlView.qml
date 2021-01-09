import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1
import "../commons"
import console 1.0

Rectangle
{
    color: '#eee'

    Flickable
    {
        anchors.fill: parent
        contentWidth: width
        contentHeight: container.height + 10
        clip: true
        ColumnLayout
        {
            id: container
            width: parent.width
            spacing: 0
            TitleRow{text:"Motor Control"}
            FieldRow{name:'Accel'; id: txtAccel; onAccepted: Console.updateField(Type.Field.A,text)}
            FieldSeparator{}
            FieldRow{name:'Velocity'; id: txtVelocity; onAccepted: Console.updateField(Type.Field.V,text)}
            FieldSeparator{}
            FieldRow
            {
                id: txtPosition
                name:'Distance'
            }
            FieldSeparator{}
            RowLayout
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Button
                {
                    Layout.fillWidth: true
                    id: btnBackward
                    text: 'CCW'
                    Material.background: Material.Orange
                    onClicked:
                    {
                        var val = parseInt(txtPosition.text);
                        Console.move(-val);
                    }
                }

                Button
                {
                    id: btnForward
                    Layout.fillWidth: true
                    text: 'CW'
                    Material.background: Material.Orange
                    onClicked:
                    {
                        var val = parseInt(txtPosition.text);
                        Console.move(val);
                    }
                }
            }

            FieldSeparator{}
            RowLayout
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Button
                {
                    id: btnStart
                    Layout.fillWidth: true
                    text: 'Start'
                    Material.background: Material.Teal
                    onClicked: Console.startMotor()
                }

                Button
                {
                    Layout.fillWidth: true
                    id: btnStop
                    text: 'Stop'
                    Material.background: Material.Teal
                    onClicked: Console.stopMotor()
                }
            }

            FieldSeparator{}

            Button
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                id: btnCalibrate
                text: 'Calibrate'
                Material.background: Material.Blue
                onClicked: Console.calibrateMotor()
            }

            TitleRow{text:"Position Regulator"}
            FieldRow{name:'pKP'; id: positionKP; onAccepted: Console.updateField(Type.Field.PKP,text)}
            FieldSeparator{}
            FieldRow{name:'pKI'; id: positionKI; onAccepted: Console.updateField(Type.Field.PKI,text)}
            FieldSeparator{}
            FieldRow{name:'pKD'; id: positionKD; onAccepted: Console.updateField(Type.Field.PKD,text)}
            FieldSeparator{}
            FieldRow{name:'vKP'; id: velopcityKP; onAccepted: Console.updateField(Type.Field.VKP,text)}
            FieldSeparator{}
            FieldRow{name:'vKI'; id: velopcityKI; onAccepted: Console.updateField(Type.Field.VKI,text)}
            FieldSeparator{}
            FieldRow{name:'KVFF'; id: positionKVFF; onAccepted: Console.updateField(Type.Field.KVFF,text)}
            FieldSeparator{}
            FieldRow{name:'KAFF'; id: positionKAFF; onAccepted: Console.updateField(Type.Field.KAFF,text)}
            FieldSeparator{}
            TitleRow{text:"Current Regulator"}
            FieldRow{name:'dKP'; id: currentDKP; onAccepted: Console.updateField(Type.Field.DKP,text)}
            FieldSeparator{}
            FieldRow{name:'dKI'; id: currentDKI; onAccepted: Console.updateField(Type.Field.DKI,text)}
            FieldSeparator{}
            FieldRow{name:'qKP'; id: currentQKP; onAccepted: Console.updateField(Type.Field.QKP,text)}
            FieldSeparator{}
            FieldRow{name:'qKI'; id: currentQKI; onAccepted: Console.updateField(Type.Field.QKI,text)}
            TitleRow{text:"Actions"}

            Button
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                id: btnLoadFields
                text: 'Load Fields'
                Material.background: Material.Teal
                onClicked: Console.loadFields()
            }

            FieldSeparator{}

            Button
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                text: 'Send Fields'
                Material.background: Material.Teal
                //onClicked: Console.stopMotor()
            }

            FieldSeparator{}
            TitleRow{text:"Update firmware"}
            Button
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                enabled: !btnOpenfile.enabled
                text: 'Enable'
                Material.background: Material.Teal
                onClicked: Console.enableBootLoader()
            }
            FieldRow{name:'Percent'; id: txtPercent; enabled: true;}
            FieldSeparator{}
            FileDialog{
                id: fileDialog;
                title: "Please choose a file";
                nameFilters: ["*.hex"];
                onAccepted: {
                    console.log("User has selected " + fileDialog.fileUrl)
                    btnDownloadFirmware.enabled = true
                    Console.updateFileName(fileDialog.fileUrl)
                    fileDialog.close()
                }
                onRejected: {
                    console.log("Canceled")
                }
            }

            RowLayout
            {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Button
                {
                    Layout.fillWidth: true
                    id: btnOpenfile
                    enabled: false
                    text: 'Open File'
                    Material.background: Material.Orange
                    onClicked:
                    {                        
                        fileDialog.open()
                    }
                }

                Button
                {
                    id: btnDownloadFirmware
                    enabled: false
                    Layout.fillWidth: true
                    text: 'Download'
                    Material.background: Material.Orange
                    onClicked:
                    {
                        btnDownloadFirmware.enabled = false
                        btnOpenfile.enabled = false
                        Console.updateFirmware()
                    }
                }
            }

        }
    }

    Connections
    {
        target: Console
        onFieldUpdated:
        {
            switch (type)
            {
                case Type.Field.PKP: positionKP.text = value; break;
                case Type.Field.PKI: positionKI.text = value; break;
                case Type.Field.PKD: positionKD.text = value; break;
                case Type.Field.VKP: velopcityKP.text = value; break;
                case Type.Field.VKI: velopcityKI.text = value; break;
                case Type.Field.KVFF: positionKVFF.text = value; break;
                case Type.Field.KAFF: positionKAFF.text = value; break;
                case Type.Field.DKP: currentDKP.text = value; break;
                case Type.Field.DKI: currentDKI.text = value; break;
                case Type.Field.QKP: currentQKP.text = value; break;
                case Type.Field.QKI: currentQKI.text = value; break;
                case Type.Field.A: txtAccel.text = value; break;
                case Type.Field.PercentDownload: txtPercent.text = value + " %"; break;
                case Type.Field.BootModeEnable:
                {
                    if(1 == value) btnOpenfile.enabled = true
                    else
                    {
                        btnOpenfile.enabled = false
                        btnDownloadFirmware.enabled = false
                        txtPercent.text = "0"
                    }
                    break;
                }
            }
        }
    }
}
