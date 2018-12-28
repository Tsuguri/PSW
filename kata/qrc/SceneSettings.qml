import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "controls"

GridLayout {
    id: root

    signal clearRectangles()
    signal simulate()
    signal changeConf()

    property bool enabled: true

    property int obstaclesCount: 0

    columns: 2
    readonly property real l1: l1textField.value
    readonly property real l2: l2textField.value

    Text {
        text: qsTr("R1")
        color: "#f5f5f5"
    }

    DoubleTextField {
        id: l1textField
        minValue: 1
        maxValue: 500
        decimals: 0
        text: "100"
        Layout.fillWidth: true
        enabled: root.enabled
    }

    Text {
        text: qsTr("R2")
        color: "#f5f5f5"
    }

    DoubleTextField {
        id: l2textField
        minValue: 1
        maxValue: 500
        decimals: 0
        text: "200"
        Layout.fillWidth: true
        enabled: root.enabled
    }

    Text {
        text: qsTr("Rects")
        color: "#f5f5f5"
    }

    Text {
        text: root.obstaclesCount
        color: "#f5f5f5"
        Layout.alignment: Qt.AlignHCenter
    }

    Button {
        id: simulateButton
        text: qsTr("Simulate")
        onClicked: root.simulate()
        enabled: root.enabled
        Layout.fillWidth: true
    }

    Button {
        id: configButton
        text: qsTr("Configure")
        onClicked: root.changeConf()
        enabled: root.enabled
        Layout.fillWidth: true
    }
}
