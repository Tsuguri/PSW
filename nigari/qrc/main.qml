import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import Nigari 1.0


ApplicationWindow {
    id: appWindow
    visible: true
    title: "Nigari"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Simulation {
        id:simulation
        cubeSide: Number.fromLocaleString(lengthInput.text)
        ro: Number.fromLocaleString(roInput.text)
    }

    property bool diagonalEnabled: diagonalCheck.checked
    property bool cubeEnabled: cubeCheck.checked
    property bool pathEnabled: true
    property bool gravityEnabled: gravityCheck.checked
    property real cubeLen: simulation.cubeSide


    Rectangle {
        id: leftMenu
        color: "gray"
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }

        Column {
            anchors.fill: parent

            Button {
                text: simulation.running ?  "Pause" : "Start"
                onClicked: simulation.toggleRun()
                width: parent.width
                ToolTip.visible: hovered
                ToolTip.text: "Start or pause simulation"
            }

            Button {
                text: "Reset"
                onClicked: {simulation.reset(); trace.reset();}
                width: parent.width
                ToolTip.visible: hovered
                ToolTip.text: "Will stop simulation and reset all data"
            }

            CheckBox{
                id: diagonalCheck
                checked: true
                text: "diagonal"
                ToolTip.visible: hovered
                ToolTip.text: "Show/hide diagonal"
            }

            CheckBox{
                id:cubeCheck 
                checked: true
                text: "cube"
                ToolTip.visible: hovered
                ToolTip.text: "Show/hide cube"
            }
            CheckBox{
                id:gravityCheck
                checked: true
                text: "gravity"
                ToolTip.visible: hovered
                ToolTip.text: "Show/hide gravity vector"
            }

            GridLayout {
                columns: 2
                width: 200

                Text {
                    text: " ro"
                }

                DecimalInput {
                    id: roInput
                    text: "1,0"
                    Layout.fillWidth: true
                    min: 0.001
                    max: 1000.0
                    decimals: 3
                }

                Text {
                    text: " cube side"
                }

                DecimalInput {
                    id: lengthInput
                    text: "1,0"
                    Layout.fillWidth: true
                    min: 0.01
                    max: 20.0
                    decimals: 3
                }
                Text {
                    text: "trace len"
                }

                SpinBox {
                    id: traceLen
                    value: 200
                    to: 2000
                    from: 1
                    editable: true
                }
            }
        }

        width: 250

    }// left menu

    Scene3D {
        id: visualization
        anchors {
            left: leftMenu.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        
        aspects: ["input","logic","render"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Entity {
            id: root

            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                nearPlane: 0.1
                farPlane: 100.0
                position: Qt.vector3d(0.0, 3.0, 12.0)
                upVector: Qt.vector3d(0.0, 1.0, 0.0)
                viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
            }

            OrbitCameraController {
                camera: camera
                linearSpeed: 100.0
                lookSpeed: 500.0
            }

            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        camera: camera
                        clearColor: "lightgray"
                    }
                },
                InputSettings {},
                DirectionalLight {
                    worldDirection: Qt.vector3d(1, -1, 1)
                },
                DirectionalLight {
                    worldDirection: Qt.vector3d(-1, -1, -1)
                }
            ]

            PhongAlphaMaterial {
                id: cubeMat
                ambient: "gray"
                alpha: 0.9
            }
            PhongAlphaMaterial {
                id: baseMat
                ambient: "darkblue"
                alpha: 0.7
            }
            PhongAlphaMaterial {
                id: gravityMat
                ambient: "red"
                alpha: 0.9
            }

            CuboidMesh {
                id: baseMesh
                xExtent: 10
                yExtent: 0.2
                zExtent: 10
            }

            CuboidMesh {
                id: cubeMesh
                property real len: cubeLen
                property var center: Qt.vector3d(len/2, len/2, len/2)
                xExtent: len
                yExtent: len
                zExtent: len
            }

            CylinderMesh {
                id: diagonalMesh
                radius: 0.05
                length: cubeMesh.len* Math.sqrt(3)
            }
            Transform {
                id: frameTransform
                rotation: simulation.rotation
            }

            Transform {
                id: cubeTransform
                translation: cubeMesh.center
            }

            Transform {
                id: diagonalTransform
                rotation: simulation.quatFromTo(Qt.vector3d(0,1,0), cubeMesh.center)
                translation: cubeMesh.center
            }

            Transform {
                id: gravityTransform
                translation: Qt.vector3d(0,-(cubeMesh.len*Math.sqrt(3))/2, 0)
                rotation: simulation.quatFromTo(Qt.vector3d(0,1,0), Qt.vector3d(0,-1,0))
            }

            Entity {
                id: base
                components: [baseMat, baseMesh]
            }

            Entity {
                id: gravity
                enabled: gravityEnabled
                components: [diagonalMesh, gravityTransform, gravityMat]
            }


            Entity {
                id: frame

                Entity {
                    id: cube
                    enabled: cubeEnabled
                    components: [cubeMat, cubeMesh, cubeTransform]
                }

                Entity {
                    id: diagonal
                    enabled: diagonalEnabled
                    components: [baseMat, diagonalMesh, diagonalTransform]
                }


                components: [frameTransform]
            }

                Trace {
                    id: trace
                    traceLen: traceLen.value
                }
        } // root entity
    
    } // Scene3D
    Connections {
        target:simulation 
        onRotationChanged: {
            var pos = simulation.quatTimesVec(simulation.rotation, Qt.vector3d(cubeLen, cubeLen, cubeLen));
            trace.newPoint(pos);
        }

    }

} // ApplicationWindow
