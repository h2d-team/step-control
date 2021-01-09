import QtQuick 2.12

Rectangle
{
    id: control
    activeFocusOnTab: true
    property alias text: textInput.text
    property alias validator: textInput.validator
    property alias textInput: textInput
    signal accepted()
    width: parent.width
    height: 30
    radius: 3
    clip: true
//    color: 'transparent'
    border.color: control.enabled ? '#ccc' : '#d5d5d5'

    onActiveFocusChanged:
    {
        if (activeFocus)
        {
            textInput.focus = true
            textInput.selectAll()
        }
    }

    TextInput
    {
        id: textInput
        anchors.centerIn: parent
        width: parent.width
        leftPadding: 5
        rightPadding: 5
        selectByMouse: true
        color: control.enabled ? '#333' : '#777'
        selectionColor: '#E91E63'
        validator: IntValidator{bottom: 0; top: 100000}
        onActiveFocusChanged:
        {
            if (activeFocus)
            {
                textInput.focus = true
                selectAll()
            }
        }
        onAccepted: control.accepted()
    }
    MouseArea
    {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        cursorShape: Qt.IBeamCursor
    }
}
