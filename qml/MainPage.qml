import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.12

Window {
    width: 340
    height: 400
    visible: true

    Item {

    id: mainItem
    anchors.fill: parent
    width: 340
    height: 400



    StackView {
        id: stackView
        x: 0
        width: 340
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 1
        initialItem: "CheckConnection.ui.qml"
    }

    ToolBar {
        id: toolBar
        y: 0
        height: parent.height/12
        anchors.left: parent.left
        anchors.right: parent.right
        Material.background: "#B972FF"

        ToolButton {
            id: drawerToolButton
            width: 48
            text: qsTr("\u2630")
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onClicked:{
                drawer.position ? drawer.close() : drawer.open()
            }

        }

        Text {
            id: titleText
            x: 95
            width: 166
            color: "#ffffff"
            text: qsTr(stackView.currentItem.title)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            font.pointSize: 22
            style: Text.Normal

            minimumPointSize: 2

            fontSizeMode: Text.Fit
            anchors.left: toolButton.right
            anchors.right: toolButton1.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            anchors.rightMargin: 10
        }
    }

}

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.66}
}
##^##*/
