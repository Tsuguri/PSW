import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import "controls"

ApplicationWindow {
    //id: appWindow
    visible: true
    title: "Haru"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Item{
        id: simulator
        property real time: 0 
        property bool running: true
    }

    header: ToolBar {
        Row {
            anchors.fill: parent
            spacing: 4

            ToolButton {
                text: "Run"
                enabled: !simulator.running
            }
            ToolButton {
                text: "Pause"
                enabled: simulator.running
            }
            ToolButton {
                text: "Reset"
            }
        }
    }

    Rectangle {
        id: background

        color: "#424242"

        anchors.fill: parent

        Item {
            id: leftPaneContainer
            width: 250

            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                spacing: 5

                ScrollableSection {
                    header: "Info"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: 280
                    Layout.minimumHeight: 50
                }

                ScrollableSection {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50

                    header: "Properties"

                }
            }
        }

        Item {
            id: centerGraphs

            width: 350

            anchors {
                right: visualization.left
                left: leftPaneContainer.right
                top: parent.top
                bottom: bottomGraphs.top
            }

            RowLayout {

                anchors.fill: parent
                anchors.margins: 5

                spacing: 5

                Section {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    header: "Forces"

                    ChartView {
                        id: forcesChart
                        anchors.fill: parent
                        antialiasing: true

                        ValueAxis {
                            id: forceTimeAxis
                            property real windowWidth: 15
                            min: simulator.time < 2 * windowWidth / 3 ? 0 : simulator.time - 2 * windowWidth / 3
                            max: simulator.time < 2 * windowWidth / 3 ? windowWidth : simulator.time + windowWidth / 3
                        }

                        ValueAxis {
                            id: forceYAxis
                            min: -5.0
                            max: 5.0
                        }

                        LineSeries {
                            id: elasticitySeries
                            name: "Elasticity"
                            axisX: forceTimeAxis
                            axisY: forceYAxis
                            useOpenGL: true
                        }

                        LineSeries {
                            id: dragSeries
                            name: "Drag"
                            axisX: forceTimeAxis
                            axisY: forceYAxis
                            useOpenGL: true
                        }

                        LineSeries {
                            id: externalSeries
                            name: "External"
                            axisX: forceTimeAxis
                            axisY: forceYAxis
                            useOpenGL: true
                        }

                    }

                }

                Section {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    header: "Configuration space"

                    ChartView {
                        id: cspaceChart
                        anchors.fill: parent
                        antialiasing: true

                        legend {
                            visible: false
                        }

                        ValueAxis {
                            id: positionAxis
                            min: -1
                            max: 1
                        }

                        ValueAxis {
                            id: velocityAxis
                            min: -1
                            max: 1
                        }

                        LineSeries {
                            id: cspaceSeries
                            name: "State"
                            axisX: positionAxis
                            axisY: velocityAxis
                            useOpenGL: true
                        }

                    }
                }

            }
        }

        Visualization {
            id: visualization
            running: simulator.running

            width: 250
            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
        }
        Item {
            id: bottomGraphs

            height: 250

            anchors {
                left: leftPaneContainer.right
                right: visualization.left
                bottom: parent.bottom
            }

            RowLayout {
                anchors.fill: parent
                anchors.topMargin: 5
                anchors.bottomMargin: 5

                spacing: 5

                Section {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    header: "Pos, Vel, Acc"

                    ChartView {
                        id: kinematicsChart
                        anchors.fill: parent
                        antialiasing: true

                        ValueAxis {
                            id: kinematicsTimeAxis
                            property real windowWidth: 15
                            min: simulator.time < 2 * windowWidth / 3 ? 0 : simulator.time - 2 * windowWidth / 3
                            max: simulator.time < 2 * windowWidth / 3 ? windowWidth : simulator.time + windowWidth / 3
                        }

                        ValueAxis {
                            id: kinematicsYAxis
                            min: -1
                            max: 1
                        }


                        LineSeries {
                            id: positionSeries
                            name: "Position"
                            axisX: kinematicsTimeAxis
                            axisY: kinematicsYAxis
                            useOpenGL: true
                        }

                        LineSeries {
                            id: velocitySeries
                            name: "Velocity"
                            axisX: kinematicsTimeAxis
                            axisY: kinematicsYAxis
                            useOpenGL: true
                        }

                        LineSeries {
                            id: accelerationSeries
                            name: "Acceleration"
                            axisX: kinematicsTimeAxis
                            axisY: kinematicsYAxis
                            useOpenGL: true
                        }

                    }
                }
            }
        }
    }
}
