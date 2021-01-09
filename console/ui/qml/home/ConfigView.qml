import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import "../commons"
import console 1.0

Rectangle
{
    color: '#eee'

    Flickable
    {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        //anchors.bottomMargin: 0
        contentWidth: width
        contentHeight: container.height + 10
        clip: true



        GridLayout
        {
            id: container
            width: parent.width
            columns: 2
            columnSpacing: 10
            SectionTitle
            {
                Layout.fillWidth: true
                text: 'Motor Control'
                icon: ""
                Layout.columnSpan: 2
            }

            Text
            {
                text: 'Acceleration'
                color: '#333'
                font.pixelSize: 15
            }
            RectangularInput
            {
                id: accel
                Layout.fillWidth: true
                text: '0'
                KeyNavigation.tab: velocity
                onAccepted: Console.updateField(Type.Field.A,text)
            }

            Text
            {
                text: 'Velocity'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: velocity
                Layout.fillWidth: true
                text: '0'
                KeyNavigation.tab: position
                KeyNavigation.backtab: accel
                onAccepted: Console.updateField(Type.Field.V,text)
            }

            Text
            {
                text: 'Position'
                color: '#333'
                font.pixelSize: 15
            }
            RectangularInput
            {
                id: position
                Layout.fillWidth: true
                text: '0'
                validator:  IntValidator{bottom: -10000; top: 10000}
                onAccepted:
                {
                    var val = parseInt(text);
                    Console.move(val);
                }
            }

            ColumnLayout
            {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                spacing: 0
                Button
                {
                    text: 'Calibrate Motor'
                    Layout.fillWidth: true
                    onClicked: Console.calibrateMotor()
                }

                Button
                {
                    text: 'Start Motor'
                    Layout.fillWidth: true
                    onClicked: Console.startMotor()
                }

                Button
                {
                    text: 'Stop Motor'
                    Layout.fillWidth: true
                    onClicked: Console.stopMotor()
                }
            }

            SectionTitle
            {
                Layout.fillWidth: true
                text: 'Position Regulator'
                icon: ""
                Layout.columnSpan: 2
            }

            Text
            {
                Layout.preferredWidth: 90
                text: 'pKP'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: positionKP
                Layout.fillWidth: true
                text: Console.pKP
                KeyNavigation.tab: positionKI
                KeyNavigation.backtab: overCurrent
                onAccepted: Console.updateField(Type.Field.PKP,text) //Test
            }

            Text
            {
                text: 'pKI'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: positionKI
                Layout.fillWidth: true
                text: Console.pKI
                KeyNavigation.tab: positionKD
                KeyNavigation.backtab: positionKP
                onAccepted: Console.updateField(Type.Field.PKI,text)
            }

            Text
            {
                text: 'pKD'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: positionKD
                Layout.fillWidth: true
                text: Console.pKD
                KeyNavigation.tab: velocityKV
                KeyNavigation.backtab: positionKI
                onAccepted: Console.updateField(Type.Field.PKD,text)
            }

            Text
            {
                text: 'vKP'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: velocityKV
                Layout.fillWidth: true
                text: Console.vKP
                KeyNavigation.tab: velocityKI
                KeyNavigation.backtab: positionKD
                onAccepted: Console.updateField(Type.Field.VKP,text)

            }

            Text
            {
                text: 'vKI'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: velocityKI
                Layout.fillWidth: true
                text: Console.vKI
                KeyNavigation.tab: positionKVFF
                KeyNavigation.backtab: velocityKV
                onAccepted: Console.updateField(Type.Field.VKI,text)

            }

            Text
            {
                text: 'KVFF'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: positionKVFF
                Layout.fillWidth: true
                text: Console.kVFF
                KeyNavigation.tab: positionKAFF
                KeyNavigation.backtab: velocityKI
                onAccepted: Console.updateField(Type.Field.KVFF,text)
            }

            Text
            {
                text: 'KAFF'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: positionKAFF
                Layout.fillWidth: true
                text: Console.kAFF
                KeyNavigation.tab: currentDKP
                KeyNavigation.backtab: positionKVFF
                onAccepted: Console.updateField(Type.Field.KAFF,text)
            }

            SectionTitle
            {
                Layout.fillWidth: true
                text: 'Current Regulator'
                icon: ""
                Layout.columnSpan: 2
            }

            Text
            {
                text: 'dKP'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: currentDKP
                Layout.fillWidth: true
                text: Console.dKP
                KeyNavigation.tab: currentDKI
                KeyNavigation.backtab: positionKAFF
                onAccepted: Console.updateField(Type.Field.DKP,text)
            }

            Text
            {
                text: 'dKI'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: currentDKI
                Layout.fillWidth: true
                text: Console.dKI
                KeyNavigation.tab: currentQKP
                KeyNavigation.backtab: currentDKP
                onAccepted: Console.updateField(Type.Field.DKI,text)
            }

            Text
            {
                text: 'qKP'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: currentQKP
                Layout.fillWidth: true
                text: Console.qKP
                KeyNavigation.tab: currentQKI
                KeyNavigation.backtab: currentDKI
                onAccepted: Console.updateField(Type.Field.QKP,text)
            }

            Text
            {
                text: 'qKI'
                color: '#333'
                font.pixelSize: 15
            }

            RectangularInput
            {
                id: currentQKI
                Layout.fillWidth: true
                text: Console.qKI
                KeyNavigation.tab: rotorZero
                KeyNavigation.backtab: currentQKP
                onAccepted: Console.updateField(Type.Field.QKI,text)
            }
        }
    }
}
