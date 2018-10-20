import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import Haru 1.0 as Haru

import "controls"

ApplicationWindow {
    //id: appWindow
    visible: true
    title: "Haru"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Haru.Simulation {
        id: simulator
    }

    Item{
        id: initialValues

        property real p0: 0
        property real dt: 0.01
        property real mass: 1
        property real flex: 0.1
        property real damping: 0.1
    }

    header: ToolBar {
        Row {
            anchors.fill: parent
            spacing: 4

            MenuItem {
                text: "Run"
                enabled: !simulator.running || simulator.paused
                width: 50
                onTriggered: {
                    if (simulator.running) {
                        simulator.togglePause()
                    } else {
                        simulator.runSimulation(
                            initialValues.dt,
                            initialValues.mass,
                            initialValues.p0,
                            initialValues.flex,
                            initialValues.damping
                        );
                    }
                }
            }
            MenuItem {
                text: "Pause"
                enabled: simulator.running && !simulator.paused
                width: 75
                onTriggered: simulator.togglePause()
            }
            MenuItem {
                text: "Reset"
                width: 75
                onTriggered: {
                    simulator.reset()
                    positionSeries.clear()
                    velocitySeries.clear()
                    accelerationSeries.clear()
                    elasticitySeries.clear()
                    dragSeries.clear()
                    externalSeries.clear()
                    cspaceSeries.clear()

                    kinematicsYAxis.min = -1
                    kinematicsYAxis.max = 1

                    forceYAxis.min = -5
                    forceYAxis.max = 5

                    velocityAxis.min = -1
                    velocityAxis.max = 1
                    positionAxis.min = -1
                    positionAxis.max = 1
                    
                }
                enabled: simulator.running
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
            springLength: 10 + simulator.position

            width: 250
            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
        }
        Item {
            id: bottomGraphs

            height: 300

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
                            property real filled: 0.9
                            min: simulator.time < filled * windowWidth ? 0 : simulator.time - filled * windowWidth
                            max: simulator.time < filled * windowWidth ? windowWidth : simulator.time + windowWidth * (1-filled)
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
                } // section 
            } //row layout
        } //bottom graph
    } //background

    Connections {
        target: simulator
        onStepMade: {
            positionSeries.append(simulator.time, simulator.position)
            velocitySeries.append(simulator.time, simulator.velocity)
            accelerationSeries.append(simulator.time, simulator.acceleration)

            kinematicsYAxis.min = Math.min(kinematicsYAxis.min, simulator.position, simulator.velocity, simulator.acceleration)
            kinematicsYAxis.max = Math.max(kinematicsYAxis.max, simulator.position, simulator.velocity, simulator.acceleration)

            //elasticitySeries.append(simulator.time, simulator.elasticityForce)
            //dragSeries.append(simulator.time, simulator.dragForce)
            //externalSeries.append(simulator.time, simulator.externalForce)

            //forceYAxis.min = Math.min(forceYAxis.min, simulator.elasticityForce, simulator.dragForce, simulator.externalForce)
            //forceYAxis.max = Math.max(forceYAxis.max, simulator.elasticityForce, simulator.dragForce, simulator.externalForce)

            cspaceSeries.append(simulator.position, simulator.velocity)

            positionAxis.min = Math.min(positionAxis.min, simulator.position)
            positionAxis.max = Math.max(positionAxis.max, simulator.position)
            velocityAxis.min = Math.min(velocityAxis.min, simulator.velocity)
            velocityAxis.max = Math.max(velocityAxis.max, simulator.velocity)
        }
    }
} // main window
