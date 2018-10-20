import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id: root

    default property alias content: content.contentData

    property string header: "Header text"

    width: 200
    height: 300

    Item {
        id: headerContainer

        height: headerText.height + 8
        width: parent.width

        anchors {
            left: parent.left
            top: parent.top
        }

        Rectangle {
            color: "#757575"
            radius: 2

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
            top: headerContainer.bottom
            bottom: parent.bottom
        }

        radius: 2

        ScrollView {
            id: content
            clip: true
            anchors {
                fill: parent
                margins: 5
            }
            contentWidth: contentChildren.length > 0 ? contentChildren[0].width : 0
        }
    }
}
