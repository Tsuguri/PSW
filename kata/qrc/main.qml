import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import Kata 1.0


ApplicationWindow {
    id: appWindow
    visible: true
    title: "Kata"

    width: 1200
    height: 800


    Simulation {
        id: simulation
        r1: settings.l1
        r2: settings.l2
    }


    Rectangle {
        id: background

        color: "#424242"

        anchors.fill: parent

        Item {
            id: rightPaneContainer

            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }

            width: 380

            ColumnLayout {
                spacing: 5

                anchors.fill: parent
                anchors.margins: 5

                SceneSettings {
                    id: settings

                    //l1: simulation.r1
                    ////l2: simulation.r2
                    //enabled: visualization.state === "unconfigured"
                    obstaclesCount: simulation.rects
                    onClearRectangles: simulation.clearRects()
                    onSimulate: {
                        simulation.run(cSpaceImagesProvider)
                    }
                }
                SimulationControl {
                    id: control
                    onStop: {
                        console.log("stupp")
                        simulation.stop();
                    }



                }
            }
        }

        Item {
            id: centerPaneContainer

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: rightPaneContainer.left
            }

                Rectangle {
                    id: backgroundRect

                    anchors.fill: parent
                    color: "lightgray"

                    //color: sceneSettings.highlightReachable ? "lightgray" : "white"

                    border {
                        width: 2
                    }

                    Behavior on border.color {
                        ColorAnimation { duration: 100 }
                    }

                    MouseArea {
                        id: mouseArea

                        anchors.fill: parent

                        cursorShape: Qt.CrossCursor
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        hoverEnabled: true

                        onPressed: {
                            if (mouse.button === Qt.LeftButton) {
                                var point = mouseArea.mapToItem(sceneRoot, mouse.x, mouse.y)
                                simulation.placeStart(point.x, point.y);
                            }

                            if(mouse.button === Qt.RightButton) {
                                var point = mouseArea.mapToItem(sceneRoot, mouse.x, mouse.y)
                                simulation.placeEnd(point.x, point.y);
                                console.log("h2h");
                            }
                        }


                        Item {
                            id: sceneRoot

                            property real toolWidth: 10

                            x: parent.width / 2
                            y: parent.height / 2

                            Rectangle {
                                color: "white"
                                readonly property real r: simulation.r1+simulation.r2
                                radius: r
                                x: -r
                                y: -r
                                width: 2 * r
                                height: 2 * r
                            }
                            Rectangle {
                                color: "lightgray"
                                readonly property real r: Math.abs(simulation.r1-simulation.r2)
                                radius: r
                                x: -r
                                y: -r
                                width: 2 * r
                                height: 2 * r
                            }

                            Arm {
                                id: movingArm
                                l1: simulation.r1
                                l2: simulation.r2
                                a1: simulation.a1
                                a2: simulation.a2
                                color: "green"
                                visible: true
                            }


                            Arm {
                                id: startArm
                                l1: simulation.r1
                                l2: simulation.r2
                                a1: simulation.startA1
                                a2: simulation.startA2
                                color: "#3F51B5"
                                opacity: 0.5
                                visible: true
                            }

                            Arm {
                                id: endArm
                                l1: simulation.r1
                                l2: simulation.r2
                                a1: simulation.endA1
                                a2: simulation.endA2
                                color: "#3F51B5"
                                opacity: 0.5
                                visible: true
                            }

                            Arm {
                                id: optionAArm
                                l1: simulation.r1
                                l2: simulation.r2
                                a1: simulation.a1a
                                a2: simulation.a2a
                                visible: simulation.showOptions
                                opacity: 0.5
                                color: "#9C27B0"
                                midAreaVisible: true
                                onMidClicked: {
                                    simulation.confirmA()
                                }
                            }

                            Arm {
                                id: optionBArm
                                l1: simulation.r1
                                l2: simulation.r2
                                a1: simulation.a1b
                                a2: simulation.a2b
                                visible: simulation.showOptions
                                opacity: 0.5
                                midAreaVisible: true
                                color: "#9C27B0"
                                onMidClicked: {
                                    simulation.confirmB()
                                }
                            }
                        } // scene root

                    } // mouse area
                } // background rect
        }
    }
} // ApplicationWindow
