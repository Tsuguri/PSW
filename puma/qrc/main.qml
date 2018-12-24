import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import Hoken 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    title: "Puma"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Shortcut {
        sequence: "Esc"
        onActivated: appWindow.close()
    }

    Simulation {
        id: simulation
        startPos: startPos.value//Qt.vector3d(startPos.xVal, startPos.yVal, startPos.zVal)
        endPos: endPos.value//Qt.vector3d(endPos.xVal, endPos.yVal, endPos.zVal)
        startEuler: startEuler.value//Qt.vector3d(startEuler.xVal, startEuler.yVal, startEuler.zVal);
        //startEuler.x: startEuler.xVal
        //startEuler.y: startEuler.yVal
        //startEuler.z: startEuler.zVal
        endEuler: endEuler.value//Qt.vector3d(endEuler.xVal, endEuler.yVal, endEuler.zVal);
        startQuat: startQuat.value

        endQuat: endQuat.value
        slerp: slerp.checked

        animationTime: animationTime.value
    }


    Rectangle {
        id: leftMenu
        color:"gray"
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: 270

        Column {
            anchors.fill: parent

            Button{
                text: simulation.running ? "Pause" : "Start"
                onClicked: simulation.toggleRun()
            }

            Button{
                text: "Reset"
                onClicked: simulation.reset()
            }

            Text {
                text: "Animation time"
            }

            DoubleTextField{
                id: animationTime
                minValue: 0
                maxValue: 600
                enabled: !simulation.running
                text: "10.0"

            }

            Text {
                text: "Animation frames"
            }

            Text {
                text: "Positions"
            }
            PositionInput {
                id: startPos
                text: "Start"
            }
            PositionInput {
                id: endPos
                text: "End  "
            }
            Text {
                text: "EulerAngles"
            }

            PositionInput {
                id: startEuler
                text: "Start"
            }

            PositionInput {
                id: endEuler
                text: "End  "
            }

            Text {
                text: "Quat"
            }
            QuaternionInput {
                text: "Start"
                id: startQuat
            }
            QuaternionInput {
                text: "End  "
                id: endQuat
            }
            CheckBox{
                id: slerp
                text: "use slerp"

            }
        } // ColumnLayout
    } // Rectangle

    Scene3D {
        id: scene3d
        anchors {
            left: leftMenu.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        anchors.margins: 0
        focus: true
        aspects: ["input", "logic", "render"]
        cameraAspectRatioMode: Scene3D.UserAspectRatio

        Entity {
            id: rootNode
            components: [frameGraph, inputSettings]

            RenderSettings {
                id: frameGraph

                activeFrameGraph: RenderSurfaceSelector {
                    id: surfaceSelector

                    Viewport {
                        id: mainViewport
                        normalizedRect: Qt.rect(0, 0, 1, 1)

                        ClearBuffers {
                            buffers: ClearBuffers.ColorDepthBuffer
                            clearColor: "lightgray"

                            LayerFilter {
                                layers: []
                            }
                        }

                        Viewport {
                            id: topLeftViewport
                            normalizedRect: Qt.rect(0, 0, 0.5, 1)

                            CameraSelector {
                                camera: camera

                                LayerFilter {
                                    layers: [ l1 ]
                                }
                            }
                        }

                        Viewport {
                            id: topRightViewport
                            normalizedRect: Qt.rect(0.5, 0, 0.5, 1)

                            CameraSelector {
                                camera: camera

                                LayerFilter {
                                    layers: [ l2 ]
                                }
                            }
                        } //viewport 2

                    } // mainViewport
                } // activeFrameGraph
            } // frameGraph (renderSettings)

            Layer { id: l1 }
            Layer { id: l2 }

            InputSettings { id: inputSettings }

            Entity {
                id: cameraSet

                Camera {
                    id: camera
                    projectionType: CameraLens.PerspectiveProjection
                    fieldOfView: 30
                    aspectRatio: scene3d.width / 2 / scene3d.height
                    nearPlane : 0.1
                    farPlane : 100.0
                    position: Qt.vector3d( 0.0, 15.0, 15.0 )
                    upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                    viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
                }

                OrbitCameraController {
                    camera: camera
                    linearSpeed: 10
                    lookSpeed: 1000
                }

            } // cameraSet

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
            Entity {
                id: sceneRoot

                Frame {
                    layers: [l1]
                    position: simulation.startPos
                    rotation: Quat.fromEulerAngles(simulation.startEuler)
                } // euler starting frame

                Frame {
                    layers: [l1]
                    position: simulation.endPos
                    rotation: Quat.fromEulerAngles(simulation.endEuler)
                } // euler end frame

                Puma {
                    id: simpleInterpolationPuma
                    layers: [l1]
                    q1: simulation.simpleQ1
                    q2: simulation.simpleQ2
                    q3: simulation.simpleQ3
                    q4: simulation.simpleQ4
                    q5: simulation.simpleQ5
                    r2: simulation.simpleR2

                }

                Frame {
                    layers: [l2]
                    position: simulation.startPos
                    rotation: simulation.startQuat
                } // quaternion starting frame

                Frame {
                    layers: [l2]
                    position: simulation.endPos
                    rotation: simulation.endQuat
                } // quaternion end frame

                Puma {
                    id: continuousInversePuma
                    layers: [l2]
                    q1: simulation.advancedQ1
                    q2: simulation.advancedQ2
                    q3: simulation.advancedQ3
                    q4: simulation.advancedQ4
                    q5: simulation.advancedQ5
                    r2: simulation.advancedR2
                }
            } // sceneRoot
        }// rootNode
    } // scene3d
}
