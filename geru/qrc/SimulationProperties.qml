import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ColumnLayout {
    id: root

    spacing: 10

    property alias mass: massSlider.value
    property alias springConstant: springConstantSlider.value
    property alias frameSpringConstant: frameSpringConstantSlider.value
    property alias viscosityConstant: viscosityConstantSlider.value
    property alias wallsSpringiness: wallsSpringinessSlider.value
    property alias showFrame: showFrameCheckBox.checked
    property alias showParticles: showParticlesCheckBox.checked
    property alias showSprings: showSpringsCheckBox.checked
    property alias showGello: showGelloCheckBox.checked
    property alias showWalls: showWallsCheckBox.checked
    //property alias mesh: meshComboBox.currentText

    Text {
        Layout.alignment: Qt.AlignHCenter
        text: qsTr("Simulation Properties")
        font.bold: true
        font.pointSize: 10
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignHCenter

        Text {
            text: "Mass: " + root.mass.toFixed(3) + " kg"
        }

        Slider {
            id: massSlider
            Layout.fillWidth: true
            from: 0.01
            to: 100
            value: 0.2
        }

        Text {
            text: "Spring: " + root.springConstant.toFixed(3)
        }

        Slider {
            id: springConstantSlider
            Layout.fillWidth: true
            from: 0.01
            to: 100
            value: 2
        }

        Text {
            text: "Frame Spring: " + root.frameSpringConstant.toFixed(3)
        }

        Slider {
            id: frameSpringConstantSlider
            Layout.fillWidth: true
            from: 0
            to: 20
            value: 5
        }

        Text {
            text: "Viscosity: " + root.viscosityConstant.toFixed(3)
        }

        Slider {
            id: viscosityConstantSlider
            Layout.fillWidth: true
            from: 0
            to: 100
            value: 0.5
        }

        Text {
            text: qsTr("Walls bouncing: " + root.wallsSpringiness.toFixed(3))
        }

        Slider {
            id: wallsSpringinessSlider
            Layout.fillWidth: true
            from: 0
            to: 1
            value: 0.2
        }

    }

    Text {
        Layout.alignment: Qt.AlignHCenter
        text: qsTr("Display Settings")
        font.bold: true
        font.pointSize: 10
    }

    GridLayout {
        Layout.alignment: Qt.AlignHCenter

        columns: 2

        CheckBox {
            id: showFrameCheckBox
            text: qsTr("Show Box")
            checkState: Qt.Checked
        }

        CheckBox {
            id: showParticlesCheckBox
            text: qsTr("Show Points")
            checkState: Qt.Unchecked
        }

        CheckBox {
            id: showSpringsCheckBox
            text: qsTr("Show Springs")
            checkState: Qt.Checked
        }

        CheckBox {
            id: showGelloCheckBox
            text: qsTr("Show Geru")
            checkState: Qt.Checked
        }
        CheckBox {
            id: showWallsCheckBox
            text: qsTr("Show Walls")
            checkState: Qt.Checked
        }

/*
        ComboBox {
            id: meshComboBox
            Layout.columnSpan: 2
            Layout.fillWidth: true
            model: ["cube", "sphere", "cylinder", "torus"]
            currentIndex: 0
        }
        */
    }

    Item {
        id: _placeholder
        Layout.fillHeight: true
    }

}
