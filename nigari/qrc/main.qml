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
    //id: appWindow
    visible: true
    title: "Nigari"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Simulation {
        id:simulation
    }

    Rectangle {
        id: leftMenu
        color: "gray"
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
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
                alpha: 0.7
            }
            PhongAlphaMaterial {
                id: baseMat
                ambient: "lightgreen"
                alpha: 0.5
            }

            SphereMesh {
                id: baseMesh
                radius: 1
            }

            CuboidMesh {
                id: cubeMesh
                property real len: 5
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

            Entity {
                id: base
                components: [baseMat, baseMesh]
            }


            Entity {
                id: frame

                Entity {
                    id: cube
                    components: [cubeMat, cubeMesh, cubeTransform]
                }

                Entity {
                    id: diagonal
                    components: [baseMat, diagonalMesh, diagonalTransform]
                }

                components: [frameTransform]
            }

        } // root entity
    
    } // Scene3D

} // ApplicationWindow
