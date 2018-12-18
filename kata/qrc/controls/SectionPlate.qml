import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    id: root

    default property alias content: content.children

    property string header: "Header text"

    width: 200
    height: 300


    RowLayout {
        id: headeContainer

        height: headerText.implicitHeight + 8

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            leftMargin: 5
            rightMargin: 5
        }

        Item {
            id: headerItem

            anchors {
                top: parent.top
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: headerText.implicitWidth + 12


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

                anchors.fill: parent

                elide: Text.ElideRight

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                color: "#f5f5f5"
                text: root.header
            }

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

        Item {
            id: content

            clip: true

            anchors {
                fill: parent
                margins: 5
            }
        }

    }

}
