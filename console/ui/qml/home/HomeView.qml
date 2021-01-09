import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import QtCharts 2.3
import "../commons"

Rectangle
{
    anchors.fill: parent
    color: '#ccc'
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0
        RowLayout
        {
            spacing: 0
            ChartSection
            {
                id: chart
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ColumnLayout
            {
                Layout.maximumWidth: 480
                Layout.fillHeight: true

                spacing: 0
                SerialSection
                {
                    id: serialSection
                    Layout.fillWidth: true
                }

                RowLayout
                {
                    spacing: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Rectangle
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        ColumnLayout
                        {
                            anchors.fill: parent
                            spacing: 0
                            LogView
                            {
                                id: logSection
                                Connections
                                {
                                    target: serialSection
                                    onClearLog: logSection.clear()
                                }
                            }
                        }
                    }

                    ControlView
                    {
                        Layout.preferredWidth: 180
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
