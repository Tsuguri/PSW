import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import Hodo 1.0 as Hodo

import "controls"

ApplicationWindow {
    //id: appWindow
    visible: true
    title: "Hodo"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Hodo.Simulation {
        id: simulator
        w: initialValues.w
        l: initialValues.l
        r: initialValues.r
    }

  

    Item{
        id: initialValues

        property real dt: Number.fromLocaleString(timeInput.text)
        property real r: Number.fromLocaleString(rInput.text)
        property real l: Number.fromLocaleString(lInput.text)
        property real w: Number.fromLocaleString(omegaInput.text)
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

                        if( !timeInput.valid || !omegaInput.valid || !rInput.valid || !lInput.valid){
                            return;
                        }

                        simulator.runSimulation(
                            initialValues.dt
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
                    cspaceSeries.clear()

                    kinematicsYAxis.min = -1
                    kinematicsYAxis.max = 1

                    velocityAxis.min = -10
                    velocityAxis.max = 10
                    positionAxis.min = -10
                    positionAxis.max = 10
                    
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
                    Layout.maximumHeight: 480
                    Layout.minimumHeight: 50

                    GridLayout{
                        width: 200
                        columns: 2

                        Text {
                            text: "Time"
                        }
                        Text {
                            text: simulator.time.toFixed(3) + " s"
                        }
                        Text {
                            text: "Mechanizm state"
                            font.bold: true
                            Layout.columnSpan: 2
                        }
                        Text {
                            text: "Position: "
                        }
                        Text {
                            text: simulator.position.toFixed(3) + " m"
                        }
                        Text {
                            text: "Velocity: "
                        }
                        Text {
                            text: simulator.velocity.toFixed(3) + " m/s"
                        }
                        Text {
                            text: "Acceleration: "
                        }
                        Text {
                            text: simulator.acceleration.toFixed(3) + " m/s2"
                        }
                        Text {
                            text: "Angle: "
                        }
                        Text {
                            text: simulator.angle.toFixed(3)
                        }
                    }
                }

                ScrollableSection {
                    id: initialState
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50

                    property bool inputEnabled: true
                    header: "Properties"

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        Text {
                            text: "Simulation"
                            font.bold: true
                            Layout.columnSpan: 2
                        }
                        Text {
                            text: "Time step: "
                        }
                        DecimalInput {
                            id: timeInput
                            text: "0.05"
                            Layout.fillWidth: true
                            min: 0.001
                            max: 1.0
                            decimals: 3
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "R: "
                        }
                        DecimalInput {
                            id: rInput
                            text: "3.0"
                            Layout.fillWidth: true
                            min: 0
                            max: 100
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "L: "
                        }
                        DecimalInput {
                            id: lInput
                            text: "6.0"
                            Layout.fillWidth: true
                            min: initialValues.r*2
                            max: 500
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "Omega: "
                        }
                        DecimalInput {
                            id: omegaInput
                            text: "0.0"
                            Layout.fillWidth: true
                            min: -10.0
                            max: 10.0
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                    }
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
                Section {
                    anchors.fill: parent
                    anchors.margins: 5
                    anchors.leftMargin: 0

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
                            min: -10
                            max: 10
                        }

                        ValueAxis {
                            id: velocityAxis
                            min: -10
                            max: 10
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

        Visualization {
            id: visualization
            running: simulator.running
            angle: simulator.angle
            position: 10
            circleRadius: simulator.r
            barLength: simulator.l

            width: 350
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

                Section {

                anchors.fill: parent
                anchors.margins: 5
                anchors.topMargin: 0
                anchors.leftMargin: 0
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

            cspaceSeries.append(simulator.position, simulator.velocity)

            positionAxis.min = Math.min(positionAxis.min, simulator.position);
            positionAxis.max = Math.max(positionAxis.max, simulator.position);

            velocityAxis.min = Math.min(velocityAxis.min, simulator.velocity);
            velocityAxis.max = Math.max(velocityAxis.max, simulator.velocity);

        }
    }
} // main window
