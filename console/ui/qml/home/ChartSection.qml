import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.3
import "../commons"

Item
{
    ChartView
    {
        id: chartView
        anchors.fill: parent
        antialiasing: true
        legend.visible: false
        margins.top: 60
        anchors.topMargin: -20
        anchors.rightMargin: -20
        anchors.leftMargin: -40
        anchors.bottomMargin: -30
        backgroundRoundness: 0
        theme:  ChartView.ChartThemeDark
        clip: true

        property alias lineP: position
        property alias linePR: pRef
        property alias lineV: velocity
        property alias lineVR: vRef

        property real indicatorFirst: 0
        property real indicatorSecond: 999

        property int pValue: 0
        property int pRefValue: 0
        property int velocityValue: 0
        property int vRefValue: 0

        property int pFValue: 0
        property int pRefFValue: 0
        property int velocityFValue: 0
        property int vRefFValue: 0

        property int pSValue: 0
        property int pRefSValue: 0
        property int velocitySValue: 0
        property int vRefSValue: 0

        Component.onCompleted: Console.initChart(position, pRef, velocity, vRef)

        function setIndicator(first_, second_)
        {
            axisX2.remove('first');
            axisX2.remove('second');

            indicatorFirst = first_;
            indicatorSecond = second_;

            axisX2.append('first', first_)
            axisX2.append('second', second_)

            pFValue = position.at(indicatorFirst).y;
            pSValue = position.at(indicatorSecond).y;

            pRefFValue = pRef.at(indicatorFirst).y;
            pRefSValue = pRef.at(indicatorSecond).y;

            velocityFValue =  velocity.at(indicatorFirst).y;
            velocitySValue =  velocity.at(indicatorSecond).y;

            vRefFValue = vRef.at(indicatorFirst).y;
            vRefSValue = vRef.at(indicatorSecond).y;
        }

        CategoryAxis
        {
            id: axisY2
            min: 0
            max: 1000
            labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
            lineVisible: false
            labelsAngle: -90
            gridLineColor: 'yellow'
            CategoryRange
            {
                label: "velocity"
                endValue: 250
            }

            CategoryRange
            {
                label: "position"
                endValue: 750
            }
        }

        CategoryAxis
        {
            id: axisX2
            min: 0
            max: 1000
            labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
            lineVisible: false
            gridLineColor: 'green'
            labelsVisible: false
            CategoryRange
            {
                id: axisX2f
                label: "first"
                endValue: 0
            }

            CategoryRange
            {
                id: axisX2s
                label: "second"
                endValue: 999
            }
            onCategoriesChanged:
            {
                //                console.log(axisX2["first"])
                //                console.log(axisX2s.endValue)
            }
        }

        ValueAxis
        {
            id: axisX
            min: 0
            max: 1000
            tickCount: 5
            labelsVisible: false
            labelFormat: " "
            minorTickCount: 3
            lineVisible: false
            //minorGridVisible: false
            minorGridLineColor: '#333'
            gridLineColor: '#444'
        }

        ValueAxis
        {
            id: axisY
            min: 0
            max: 1000
            tickCount: 5
            labelsVisible: false
            labelFormat: " "
            minorTickCount: 3
            lineVisible: false
            //minorGridVisible: false
            minorGridLineColor: '#333'
            gridLineColor: '#444'
            //gridLineColor: 'green'
        }

        LineSeries
        {
            id: position
            name: "p"
            axisX: axisX2
            axisY: axisY2
            color: 'red'
            Component.onCompleted:
            {
                axisX.applyNiceNumbers()
                axisY.applyNiceNumbers()
            }
            onPointsReplaced:
            {
                chartView.pValue = position.at(999).y
                chartView.pFValue = position.at(chartView.indicatorFirst).y
                chartView.pSValue = position.at(chartView.indicatorSecond).y
            }
        }

        LineSeries
        {
            id: pRef
            name: "pRef"
            axisX: axisX
            axisY: axisY
            color: 'blue'
            Component.onCompleted:
            {
                axisX.applyNiceNumbers()
                axisY.applyNiceNumbers()
            }
            onPointsReplaced:
            {
                chartView.pRefValue = pRef.at(999).y
                chartView.pRefFValue = pRef.at(chartView.indicatorFirst).y
                chartView.pRefSValue = pRef.at(chartView.indicatorSecond).y
            }
        }

        LineSeries
        {
            id: velocity
            name: "v"
            axisX: axisX
            axisY: axisY
            color: "green"
            Component.onCompleted:
            {
                axisX.applyNiceNumbers()
                axisY.applyNiceNumbers()
            }
            onPointsReplaced:
            {
                chartView.velocityValue = velocity.at(999).y
                chartView.velocityFValue = velocity.at(chartView.indicatorFirst).y
                chartView.velocitySValue = velocity.at(chartView.indicatorSecond).y
            }
        }

        LineSeries
        {
            id: vRef
            name: "vRef"
            axisX: axisX
            axisY: axisY
            color: "cyan"
            Component.onCompleted:
            {
                axisX.applyNiceNumbers()
                axisY.applyNiceNumbers()
            }
            onPointsReplaced:
            {
                chartView.vRefValue = vRef.at(999).y
                chartView.vRefFValue = vRef.at(chartView.indicatorFirst).y
                chartView.vRefSValue = vRef.at(chartView.indicatorSecond).y
            }
        }

        Rectangle
        {
            id: rect
            color: "#666"
            opacity: 0.4
            visible: false
        }

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: if(mouse.button & Qt.RightButton) {parent.zoomReset()}            
            property var vX: 0
            property var vY: 0
            //onPressed: {rect.x = mouseX; rect.y = mouseY; rect.visible = true}
            onPressed: {vX = mouseX; vY = mouseY; rect.visible = true}
            //onMouseXChanged: {rect.width = mouseX - rect.x}
            //onMouseYChanged: {rect.height = mouseY - rect.y}
            onMouseXChanged:
            {
                if (mouseX < rect.x) rect.x = mouseX;
                else rect.x = vX;
                rect.width = Math.abs(mouseX - vX);
            }
            onMouseYChanged:
            {
                if (mouseY < rect.y) rect.y = mouseY;
                else rect.y = vY;
                rect.height = Math.abs(mouseY - vY);
            }
            onReleased:
            {
                var w = (rect.width/chartView.plotArea.width)* (axisX.max - axisX.min)
                //var h = (rect.height/chartView.plotArea.height)* (axisY.max - axisY.min)
                if (w<5) {rect.x = chartView.plotArea.x; rect.width = chartView.plotArea.width*5/(axisX.max - axisX.min);}
                //if (h<20) {rect.y = chartView.plotArea.y; rect.height = chartView.plotArea.height*20/(axisY.max - axisY.min);}
                parent.zoomIn(Qt.rect(rect.x, rect.y, rect.width, rect.height))
                rect.visible = false
            }
        }
    }

    Row
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.leftMargin: 30
        anchors.rightMargin: 30
        spacing: 20

        height: 40

        ColumnLayout
        {
            width: parent.width/4
            RowLayout
            {
                spacing: 5
                Layout.fillHeight: true
                Rectangle
                {
                    Layout.preferredWidth: 15
                    Layout.preferredHeight: 15
                    color: "red"

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: chartView.lineP.visible = !chartView.lineP.visible
                    }
                }

                Label
                {
                    text: 'p'
                    font.bold: true
                    color: '#eee'
                    font.pixelSize: 16
                }

            }

            Label
            {
                text: !Console.pause_ ? '['+ chartView.pValue + ']' : '[' + chartView.pFValue + ',' + chartView.pSValue+ ']'
                color: '#ccc'
            }
        }

        ColumnLayout
        {
            width: parent.width/4
            RowLayout
            {
                spacing: 5
                Layout.fillHeight: true
                Rectangle
                {
                    Layout.preferredWidth: 15
                    Layout.preferredHeight: 15
                    color: "blue"

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: chartView.linePR.visible = !chartView.linePR.visible
                    }
                }

                Label
                {
                    text: "pRef"
                    font.bold: true
                    color: '#eee'
                    font.pixelSize: 16
                }
            }
            Label
            {
                text: !Console.pause_ ? '['+chartView.pRefValue+']' : '[' + chartView.pRefFValue + ',' + chartView.pRefSValue+ ']'
                color: '#ccc'
            }
        }

        ColumnLayout
        {
            width: parent.width/4
            RowLayout
            {
                spacing: 5
                Layout.fillHeight: true
                Rectangle
                {
                    Layout.preferredWidth: 15
                    Layout.preferredHeight: 15
                    color: "green"
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: chartView.lineV.visible = !chartView.lineV.visible
                    }
                }

                Label
                {
                    text: "v"
                    font.bold: true
                    color: '#eee'
                    font.pixelSize: 16
                }

            }
            Label
            {
                text: !Console.pause_ ? '['+ chartView.velocityValue + ']' : '[' + chartView.velocityFValue + ',' + chartView.velocitySValue+ ']'
                color: '#ccc'
            }
        }

        ColumnLayout
        {
            width: parent.width/4
            RowLayout
            {
                spacing: 5
                Layout.fillHeight: true
                Rectangle
                {
                    Layout.preferredWidth: 15
                    Layout.preferredHeight: 15
                    color: "cyan"
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: chartView.lineVR.visible = !chartView.lineVR.visible
                    }
                }

                Label
                {
                    text: "vRef"
                    font.bold: true
                    color: '#eee'
                    font.pixelSize: 16
                }

            }
            Label
            {
                text: !Console.pause_ ? '['+ chartView.vRefValue + ']' : '[' + chartView.vRefFValue + ',' + chartView.vRefSValue+ ']'
                color: '#ccc'
            }
        }
    }

    RangeSlider
    {
        id: rangeSlider
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 5

        from: 0
        to: 999
        stepSize: 1
        snapMode: RangeSlider.SnapOnRelease
        first.value: 0
        second.value: 999

        enabled: Console.pause_

        first.onPressedChanged:
        {
            if (!first.pressed) chartView.setIndicator(first.value, second.value)
        }
        second.onPressedChanged:
        {
            if (!second.pressed) chartView.setIndicator(first.value, second.value)
        }
    }
}


