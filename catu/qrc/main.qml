
import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import Catu 1.0 as Catu

ApplicationWindow {
    visible: true
    title: "Catu"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600


    FileDialog {
        id: openFileDialog
        title: "Wybierz plik ze sciezka"
        folder: shortcuts.documents
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        onAccepted: {
            program.addFile(openFileDialog.fileUrl)
        }
    }

    Item {
        id: program

        function addFile(file){
            console.log("Adding file: "+ file)
        }

        function addTool(isSphere, radius){
            console.log("Adding tool: "+ radius + " " + isSphere)
        }


        property bool running: true
    }
    header: ToolBar {
        Row{
            ToolButton {
                text: "Load file"
                onClicked: openFileDialog.open()
            }
            ToolButton {
                text: "Reset state"
                onClicked: {
                    program.running = !program.running
                }
            }
            ToolButton {
                text: "Material options"
                enabled: !program.running
            }
        }
    }

    Item {
        id: leftPanel
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: 200
    } 

    Scene3D {
        id: visualization
        anchors.margins: 0
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            left: leftPanel.right
        }
        focus: true
        aspects: ["input", "logic", "render"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Entity {
            id: root
            objectName: "root"

            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                nearPlane: 0.1
                farPlane: 1000.0
                position: Qt.vector3d(0.0, 200.0, 200.0)
                upVector: Qt.vector3d(0.0, 1.0, 0.0)
                viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
            }

            OrbitCameraController {
                camera: camera
                linearSpeed: 1000.0
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
                    worldDirection: Qt.vector3d(0.5, -1, 1)
                },
                DirectionalLight {
                    worldDirection: Qt.vector3d(-1, -1, -0.8)
                }
            ]

            GoochMaterial {
                id: tableMaterial
            }
            CuboidMesh {
                id: tableMesh
                xExtent: 300
                yExtent: 1
                zExtent: 300
            }

            Entity {
                id: table
                components: [tableMesh, tableMaterial]
            }

            Catu.Material {
                id: millingMaterial
            }

        } // root Entity
    } // Scene3d
}
