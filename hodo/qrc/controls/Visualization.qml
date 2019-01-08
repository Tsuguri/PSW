import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Rectangle {
    function degToRad(a) {
        return Math.PI * a / 180.0
    }

    function radToDeg(a) {
        return 180.0 * a / Math.PI
    }
    function barStartPosition(R, L, a) {
        a = degToRad(a)
        return Qt.vector2d(
            R * Math.cos(a),
            R * Math.sin(a)
        )
    }

    function barEndPosition(R, L, a) {
        a = degToRad(a)
        return Qt.vector2d(
            R * Math.cos(a) + barProjection(R, L, a),
            0
        )
    }

    function barProjection(R, L, a) {
        return Math.sqrt(L * L - R * R * Math.sin(a) * Math.sin(a))
    }


    id: root
    property bool running: false
    property real position: 0.0
    property real angle: 0.0
    property real barLength: 10
    property real circleRadius: 4

    property vector2d barEnd: root.barEndPosition(root.circleRadius*10, root.barLength*10, root.angleDegree)
    property vector2d barStart: root.barStartPosition(root.circleRadius*10, root.barLength*10, root.angleDegree)

    property real angleDegree: radToDeg(angle)

    border {
        width: 5
        color: running ? "lightgreen" : "lightblue"
    } // border

	Rectangle {
		id: axis

		anchors.centerIn: parent

		height: root.height
		width: 2

		color: "gray"
	}

    Rectangle {
        id: mass
        width: 20
        height: 20
        color: "darkgray"
        transform: [
            Translate {
                x: root.width/2 - mass.width/2
                y: 150 - (root.barEnd.x- 150) - mass.height/2
            }
        ]
    }

    Rectangle {
        id: mass2
        width: 8
        height: 8
        color: "red"
        transform: [
            Translate {
                x: root.barStart.y - mass2.width/2 + root.width/2
                y: -root.barStart.x - mass2.height/2 + 150
            }
        ]
    }
    Rectangle {
        id: circle
        width: 10*circleRadius * Math.sqrt(2)
        height: 10*circleRadius * Math.sqrt(2)
        color: "darkgray"
        antialiasing: true
        transform: [
            Rotation {
                angle: root.angleDegree
                origin {
                    x: circle.width/2
                    y: circle.height/2
                }
            },
            Translate {
                x: root.width/2 - circle.width/2
                y: 150 - circle.height/2
            }
        ]
    }

    Rectangle {
        id: circ2
        width: circle.width
        height: circle.height
        color: "darkgray"
        antialiasing: true
        transform: [
            Rotation {
                angle: 45+root.angleDegree
                origin {
                    x: circ2.width/2
                    y: circ2.height/2
                }
            },
            Translate {
                x: root.width/2 - circ2.width/2
                y: 150 - circle.height/2
            }
        ]
    }

}
