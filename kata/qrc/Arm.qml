import QtQuick 2.7

Rectangle {
    id: root
    property real toolWidth: 6
    property real l1: 100
    property real l2: 100
    property real a1: 0
    property real a2: 0

    property bool midAreaVisible: false

    antialiasing: true
    height: root.toolWidth
    width: root.l1 + root.toolWidth
    color: "red"

    signal midClicked()

    transform: [
        Translate {
            x: -root.toolWidth / 2
            y: -root.toolWidth / 2
        },
        Rotation {
            angle: root.a1
        }
    ]

    border {
        width: 1
    }

    Rectangle {
        id: baseDot
        width: 2 * root.toolWidth
        height: 2 * root.toolWidth

        radius: root.toolWidth

        border {
            width: 1
        }

        transform: [
            Translate {
                x: -root.toolWidth / 2
                y: -root.toolWidth / 2
            }
        ]

    }

    Rectangle {
        antialiasing: true

        color: root.color

        height: root.toolWidth
        width: root.l2 + root.toolWidth

        border {
            width: 1
        }

        transform: [
            Rotation {
                angle: root.a2
                origin {
                    x: root.toolWidth / 2
                    y: root.toolWidth / 2
                }
            },
            Translate {
                x: root.l1
            }
        ]
    }

    Rectangle {
        id: midDot
        width: 2 * root.toolWidth
        height: 2 * root.toolWidth

        radius: root.toolWidth

        border {
            width: 1
        }

        transform: [
            Translate {
                x: root.l1
            },
            Translate {
                x: -root.toolWidth / 2
                y: -root.toolWidth / 2
            }
        ]

    }

    Rectangle {
        id: clickArea

        visible: root.midAreaVisible

        antialiasing: true

        opacity: 0.5

        width: 2 * radius
        height: 2 * radius

        radius: 5 * root.toolWidth

        color: midMouseArea.containsMouse ? root.color : "white"

        transform: [
            Translate {
                x: root.l1
            },
            Translate {
                x: -clickArea.radius + root.toolWidth / 2
                y: -clickArea.radius + root.toolWidth / 2
            }
        ]

        MouseArea {
            id: midMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.midClicked(mouse)
        }

    }

}