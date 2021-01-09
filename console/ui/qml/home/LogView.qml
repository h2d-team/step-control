import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12

Rectangle
{
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: '#444'
    function clear()
    {
        model.clear();
    }
    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 0

        ListView
        {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 5
            model: ListModel{id: model}
            clip: true

            delegate: RowLayout
            {
                width: parent.width

                Label
                {
                    //Layout.preferredWidth: 60
                    color: '#777'
                    text: model.time
                    font.bold: true
                    font.pixelSize: 15
                }

                Label
                {
                    Layout.fillWidth: true
                    color: 'white'
                    text: model.text
                    wrapMode: Text.Wrap
                    font.pixelSize: 15
                }
            }


            Connections
            {
                target: Console
                onPrintMessage:
                {
                    model.append({"time": time, "text": text})
                    if (model.count > 100) model.remove(0, model.count - 50);
                    listView.positionViewAtEnd()
                }
            }
        }
    }
}
