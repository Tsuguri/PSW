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
        err: initialValues.err
    }

  

    Item{
        id: initialValues

        property real dt: Number.fromLocaleString(timeInput.text)
        //property alias r: rInput.value
        //property alias l: lInput.value
        property real r: Number.fromLocaleString(rInput.text)
        property real l: Number.fromLocaleString(lInput.text)
        property real w: Number.fromLocaleString(omegaInput.text)
        property real err: Number.fromLocaleString(errInput.text)

    function resizePlot(series, axis){
        let min = 1;
        let max = -1;

        for(let i =0; i<series.count; i++)
        {
            let pt = series.at(i)
            if(min > pt.y){
                min = pt.y
            }
            if (max < pt.y) {
                max = pt.y
            }
        }


        axis.min=min
        axis.max=max

    }
    function resize() {
        console.log("resizing")
            resizePlot(positionSeries, posYAxis)
            resizePlot(velocitySeries, velocityYAxis)
            resizePlot(accelerationSeries, kinematicsYAxis)


    }
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

                        if( !timeInput.valid || !omegaInput.valid){
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

                    posYAxis.min = -1
                    posYAxis.max = 1

                    velocityYAxis.min = -1
                    velocityYAxis.max=1

                    velocityAxis.min = 0
                    velocityAxis.max =0
                    positionAxis.min = 0
                    positionAxis.max =0
                    
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
                            id:rInput 
                            text: "3.0"
                            Layout.fillWidth: true
                            min: 0.0001
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
                            min: 0.001
                            max: 200
                            decimals: 2
                            enabled: initialState.inputEnabled
                        }
                        /*Slider {
                            id: rInput
                            Layout.fillWidth: true
                            from: 0.0
                            to: 100
                            value: 3.0
                        }
                        Text {
                            text: "L: "
                        }
                        Slider {
                            id: lInput
                            Layout.fillWidth:true
                            from: rInput.value*2
                            to: 200
                            value: 6.0
                        }
                        */
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
                        }
                        Text {
                            text: "Err: "
                        }
                        DecimalInput {
                            id: errInput
                            text: "0.0"
                            Layout.fillWidth: true
                            min:0.0
                            max:20.0
                            decimals: 2
                        }
                        Button {
                            text:" adjust plots"
                            onClicked: initialValues.resize()
                        }
                    }
                }
            }
        } // left pane container

        Item {
            id: centerGraphs
            width: 350

            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
            
            ColumnLayout {
                id: right
                anchors.fill: parent

                Section{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.rightMargin: 5
                    Layout.topMargin: 5
                    header: "Visualiztion"
                Visualization {
                    id: visualization
                    running: simulator.running
                    angle: simulator.angle
                    position: 10
                    circleRadius: simulator.r
                    barLength: simulator.l
        
                    width: 350
                }
            }
                Section {
                    header: "Configuration space"
                    Layout.fillWidth:true
                    Layout.rightMargin: 5
                    Layout.bottomMargin: 5

                    ChartView {
                        id: cspaceChart
                        anchors.fill: parent
                        antialiasing: true

                        legend {
                            visible: false
                        }

                        ValueAxis {
                            id: positionAxis
                            min: 0
                            max: 0
                        }

                        ValueAxis {
                            id: velocityAxis
                            min: 0
                            max: 0
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

        Item {
            id: bottomGraphs


            anchors {
                left: leftPaneContainer.right
                right: centerGraphs.left
                bottom: parent.bottom
                top: parent.top
            }

                Section {

                anchors.fill: parent
                anchors.margins: 5
                anchors.topMargin: 5
                anchors.leftMargin: 0
                    header: "Pos, Vel, Acc"

                    ColumnLayout{
                        anchors.fill:parent
                        ChartView {

                            Layout.fillWidth: true
                            Layout.preferredHeight: 300
                            antialiasing: true
                            ValueAxis {
                                id: posTimeAxis
                                property real windowWidth: 15
                                property real filled: 0.9
                                min: simulator.time < filled * windowWidth ? 0 : simulator.time - filled * windowWidth
                                max: simulator.time < filled * windowWidth ? windowWidth : simulator.time + windowWidth * (1-filled)
                            }
                            ValueAxis {
                                id: posYAxis
                                min: -1
                                max: 1
                            }
                            LineSeries {
                                id: positionSeries
                                name: "Position"
                                axisX: posTimeAxis
                                axisY: posYAxis 
                                useOpenGL: true
                            }
                        }
                        ChartView {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 300
                            antialiasing: true
                            ValueAxis {
                                id: velocityTimeAxis
                                property real windowWidth: 15
                                property real filled: 0.9
                                min: simulator.time < filled * windowWidth ? 0 : simulator.time - filled * windowWidth
                                max: simulator.time < filled * windowWidth ? windowWidth : simulator.time + windowWidth * (1-filled)
                            }

                            ValueAxis {
                                id: velocityYAxis
                                min: -1
                                max: 1
                            }
                            LineSeries {
                                id: velocitySeries
                                name: "Velocity"
                                axisX:velocityTimeAxis 
                                axisY:velocityYAxis 
                                useOpenGL: true
                            }
                        }
                        ChartView {
                            Layout.fillWidth:true
                            Layout.preferredHeight: 300
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
                                id: accelerationSeries
                                name: "Acceleration"
                                axisX: kinematicsTimeAxis
                                axisY: kinematicsYAxis
                                useOpenGL: true
                            }

                        }
                    } // column
                } // section 
        } //bottom graph
    } //background

    Connections {
        target: simulator
        onStepMade: {
            var max = 300
            positionSeries.append(simulator.time, simulator.position)
            velocitySeries.append(simulator.time, simulator.velocity)
            accelerationSeries.append(simulator.time, simulator.acceleration)

            if(positionSeries.count > max)
            {
                positionSeries.remove(0)
                velocitySeries.remove(0)
                accelerationSeries.remove(0)
            }

            kinematicsYAxis.min = Math.min(kinematicsYAxis.min, simulator.acceleration)
            kinematicsYAxis.max = Math.max(kinematicsYAxis.max, simulator.acceleration)

            velocityYAxis.min = Math.min(velocityYAxis.min, simulator.velocity)
            velocityYAxis.max = Math.max(velocityYAxis.max, simulator.velocity)

            posYAxis.min = Math.min(posYAxis.min, simulator.position)
            posYAxis.max = Math.max(posYAxis.max, simulator.position)

            cspaceSeries.append(simulator.position, simulator.velocity)

            if(cspaceSeries.count > max) {
                cspaceSeries.remove(0)

            }

            positionAxis.min = Math.min(positionAxis.min, simulator.position);
            positionAxis.max = Math.max(positionAxis.max, simulator.position);

            velocityAxis.min = Math.min(velocityAxis.min, simulator.velocity);
            velocityAxis.max = Math.max(velocityAxis.max, simulator.velocity);

        }
    }
} // main window
