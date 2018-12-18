import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ColumnLayout {
    id: root

    property bool enabled: true

    signal close()
    signal stop()

    Text {
        text: qsTr("CSpace Image")
        color: "#f5f5f5"
    }

    Image {
        id: cSpaceImage
        Layout.fillWidth: true

        fillMode: Image.PreserveAspectFit

        source: "image://cSpaceImagesProvider/cSpace"

        cache: false

        mipmap: true

        Connections {
            target: cSpaceImagesProvider
            onImageChanged: {
                cSpaceImage.source = ""
                cSpaceImage.source = "image://cSpaceImagesProvider/cSpace"
            }
        }
    }

    Item {
        Layout.fillHeight: true
    }

    RowLayout {
        Layout.fillWidth: true

        Button {
            enabled: root.enabled
            Layout.fillWidth: true
            text: qsTr("Stop")
            onClicked: root.stop()
        }
    }

}
