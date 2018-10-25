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

    Haru.MathFormula {
        id: externalForceFormula
    }

    Haru.MathFormula {
        id: originMovementFormula
    }

    Item{
        id: initialValues

        property real p0: Number.fromLocaleString(positionInput.text)
        property real v0: Number.fromLocaleString(velocityInput.text)
        property real dt: Number.fromLocaleString(timeInput.text)
        property real mass: Number.fromLocaleString(massInput.text)
        property real flex: Number.fromLocaleString(flexInput.text)
        property real damping: Number.fromLocaleString(dampInput.text)
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

                        if( !positionInput.valid || !velocityInput.valid || !timeInput.valid || !massInput.valid || !flexInput.valid || !dampInput.valid || !originMovementFormula.valid || !externalForceFormula){
                            return;
                        }

                        simulator.runSimulation(
                            initialValues.dt,
                            initialValues.mass,
                            initialValues.p0,
                            initialValues.v0,
                            initialValues.damping,
                            initialValues.flex,
                            externalForceFormula,
                            originMovementFormula
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
                    flexSeries.clear()
                    dragSeries.clear()
                    externalSeries.clear()
                    cspaceSeries.clear()

                    kinematicsYAxis.min = -1
                    kinematicsYAxis.max = 1

                    forceYAxis.min = -5
                    forceYAxis.max = 5

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
                            text: "Spring state"
                            font.bold: true
                            Layout.columnSpan: 2
                        }
                        Text {
                            text: "Mass" 
                        }
                        Text {
                            text: simulator.mass.toFixed(3) + " kg"
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
                            text: "Forces"
                            font.bold: true
                            Layout.columnSpan: 2
                        }
                        Text {
                            text: "Elasticity"
                        }
                        Text {
                            text: simulator.flexForce.toFixed(3) + " N/kg"
                        }
                        Text {
                            text: "Damping"
                        }
                        Text {
                            text: simulator.dampingForce.toFixed(3) + " N/kg"
                        }
                        Text {
                            text: "External"
                        }
                        Text {
                            text: simulator.externalForce.toFixed(3) + " N/kg"
                        }
                    }

                }

                ScrollableSection {
                    id: initialState
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50

                    property bool inputEnabled: !simulator.running
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
                            text: "0,1"
                            Layout.fillWidth: true
                            min: 0.001
                            max: 1.0
                            decimals: 3
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "Weight"
                            font.bold: true
                            Layout.columnSpan: 2
                        }
                        Text {
                            text: "Mass: "
                        }
                        DecimalInput {
                            id: massInput
                            text: "2,0"
                            Layout.fillWidth: true
                            min: 0
                            max: 100
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "Position: "
                        }
                        DecimalInput {
                            id: positionInput
                            text: "0,0"
                            Layout.fillWidth: true
                            min: -10.0
                            max: 10.0
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "Velocity: "
                        }
                        DecimalInput {
                            id: velocityInput
                            text: "0,0"
                            Layout.fillWidth: true
                            min: -10.0
                            max: 10.0
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "Forces"
                            font.bold: true
                            Layout.columnSpan: 2
                        }
                        Text {
                            text: "Elasticity:"
                        }
                        DecimalInput {
                            id: flexInput
                            text: "0,3"
                            Layout.fillWidth: true
                            min: 0.01
                            max: 10.0
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "Damping:"
                        }
                        DecimalInput {
                            id: dampInput
                            text: "0,1"
                            Layout.fillWidth: true
                            min: 0.0
                            max: 10.0
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        Text {
                            text: "External force:"
                        }
                        TextField {
                            id: externalForceField
                            Layout.fillWidth: true
                            text: "0,0"
                            placeholderText: qsTr("Enter formula")
                            enabled: initialState.inputEnabled
                            background: Rectangle {
                                border {
                                    width: 1
                                    color: externalForceFormula.valid ? "darkgray" : "red"
                                }
                            }
                            onTextChanged: externalForceFormula.text = externalForceField.text
                        }

                        Text {
                            text: "Origin movement: "
                        }
                        TextField {
                            id: originMovementField
                            Layout.fillWidth: true
                            text: "0,0"
                            placeholderText: qsTr("Enter formula")
                            enabled: initialState.inputEnabled
                            background: Rectangle {
                                border {
                                    width: 1
                                    color: originMovementFormula.valid ? "darkgray" : "red"
                                }
                            }
                            onTextChanged: originMovementFormula.text = originMovementField.text
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
                            property real fill: 0.9

                            min: simulator.time < fill * windowWidth ? 0 : simulator.time - fill * windowWidth
                            max: simulator.time < fill * windowWidth ? windowWidth : simulator.time + (1-fill) * windowWidth
                        }

                        ValueAxis {
                            id: forceYAxis
                            min: -5.0
                            max: 5.0
                        }

                        LineSeries {
                            id: flexSeries
                            name: "Elasticity"
                            axisX: forceTimeAxis
                            axisY: forceYAxis
                            useOpenGL: true
                        }

                        LineSeries {
                            id: dragSeries
                            name: "Dump"
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
        }

        Visualization {
            id: visualization
            running: simulator.running
            springLength: 10 + simulator.position + simulator.originPosition
            basePosition: simulator.originPosition

            width: 250
            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
        }
        Item {
            id: bottomGraphs

            height: 600

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

            flexSeries.append(simulator.time, simulator.flexForce)
            dragSeries.append(simulator.time, simulator.dampingForce)
            externalSeries.append(simulator.time, simulator.externalForce)

            forceYAxis.min = Math.min(forceYAxis.min, simulator.flexForce, simulator.dampingForce, simulator.externalForce)
            forceYAxis.max = Math.max(forceYAxis.max, simulator.flexForce, simulator.dampingForce, simulator.externalForce)

            cspaceSeries.append(simulator.position, simulator.velocity)

            positionAxis.min = Math.min(positionAxis.min, simulator.position);
            positionAxis.max = Math.max(positionAxis.max, simulator.position);

            velocityAxis.min = Math.min(velocityAxis.min, simulator.velocity);
            velocityAxis.max = Math.max(velocityAxis.max, simulator.velocity);

        }
    }
} // main window
