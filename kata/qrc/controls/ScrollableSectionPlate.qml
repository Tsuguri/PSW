import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id: root

    default property alias content: content.contentData

    property string header: "Header text"

    width: 200
    height: 300

    Item {
        id: headeContainer

        height: headerText.height + 8
        width: headerText.width + 8

        anchors {
            left: parent.left
            leftMargin: 5
            top: parent.top
        }

        Rectangle {
            color: "#757575"
            radius: 5

            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }

            height: parent.height + 5
        }

        Text {
            id: headerText

            anchors.centerIn: parent

            color: "#f5f5f5"
            text: root.header
        }

    }

    Rectangle {
        id: contentRect

        color: "#616161"

        anchors {
            left: parent.left
            right: parent.right
            top: headeContainer.bottom
            bottom: parent.bottom
        }

        radius: 3

        ScrollView {
            id: content

            clip: true

            anchors {
                fill: parent
                margins: 5
            }

            contentWidth: contentChildren[0].width

        }

    }

}
