
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
    id: mainWindow
        function fileName(file){
            var s
            if (file.startsWith("file:///")) {
                var k = file.charAt(9) === ':' ? 8 : 7
                s = file.substring(k)
            } else {
                s =file 
            }
            return decodeURIComponent(s);
        }
    visible: true
    title: "Catu"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Shortcut {
        sequence: "Ctrl+w"
        onActivated: mainWindow.close()
    }

    Shortcut {
        sequence: "Ctrl+t"
        onActivated: openFileDialog.open()
        enabled: !millingManager.active
    }


    FileDialog {
        id: openFileDialog
        title: "Wybierz plik ze sciezka"
        folder: shortcuts.documents
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        onAccepted: {
            pathManager.AddPath(openFileDialog.fileUrl)
        }
    }

    Item {
        id: program

        function addTool(isSphere, radius){
            console.log("Adding tool: "+ radius + " " + isSphere)
        }

        property bool materialVisible: true
        property bool pathsVisible: true
        property alias material: millingMaterial
    }

    Catu.ToolManager {
        id: tools

        tools: [
            Catu.Mill {}
        ]
    }

    Catu.PathManager {
        id: pathManager
        toolManager: tools
        paths:[]
    }

    Catu.MillingManager {
        id: millingManager
        paths: pathManager
        tools: tools
        material: millingMaterial
    }

    MessageDialog {
        id: materialResetConfirmationDialog
        title: "Material contains milled areas"
        text: "Are you sure you want to reset material? It will destroy all modified areas"
        standardButtons: StandardButton.Yes | StandardButton.Abort
        onYes: {
            console.log("Destroying material")
        }
        onRejected: {
            console.log("not destroying material")
        }
    }
    
    Popup {
        id: toolConfigurationPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        modal: true
        focus: true

        width: parent.width - 600
        height: 650
        x: 300
        y: 100
        Rectangle {
            anchors.fill: parent
            
            Component {
                id: toolDelegate

                Rectangle{
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    border {
                        width: 2
                        color: "gray"
                    }
                    color: "lightgray"
                    radius: 2
                    height: 80

                    GridLayout {
                        anchors.fill: parent
                        columns: 4
                        anchors.margins: 4

                        Text{
                            width: 100
                            horizontalAlignment: Text.AlignHCenter
                            text: toolRadius
                        }
                        Text{
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                            text: type
                        }
                        Text{
                            Layout.fillWidth: true
                        }
                        Button{
                            text: "Remove"
                            enabled: !inUse
                            onClicked: tools.RemoveTool(index)
                            padding: 2
                            implicitWidth: 100
                            background: Rectangle {
                                radius: 2
                                color: "lightgray"
                                border {
                                    width: 2
                                    color: "gray"
                                }
                            }
                        }
                    }
                }
            }

            GridLayout {
                id:addToolButton
                columns: 3
                ComboBox {
                    id: newToolType
                    model: ["Spheric", "Flat"]
                }
                SpinBox {
                    id: newToolRadius
                    to: 100
                    from: 1
                    value: 10
                    editable: true
                    Layout.fillWidth: true

                }
                Button {
                    text: "Add tool"
                    onClicked: {
                        tools.AddTool(newToolRadius.value, newToolType.currentIndex)
                    }
                }
            }
            ListView{
                anchors {
                    left: parent.left
                    top: addToolButton.bottom
                    right: parent.right
                    bottom: parent.bottom
                }
                model: tools.tools
                delegate: toolDelegate
                spacing: 2
            } // listview
        } // rectangle
    }// tool conf popup

    Popup {
        id: materialConfigurationPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        modal:true
        focus: true


        width: parent.width - 400
        height: 350

        x: 200
        y: 100
        Rectangle {
            anchors.fill: parent

            GridLayout {
                columns: 2
                rows: 7
                anchors.fill:parent

                Text{
                    text: "x resolution"
                }
                SpinBox {
                    id: xRes
                    to: 1500
                    from: 10
                    value: 600
                    editable: true
                    Layout.fillWidth: true
                }

                Text {
                    text: "z resolution"
                }

                SpinBox {
                    id: yRes
                    to: 1500
                    from: 10
                    value: 600
                    editable: true
                    Layout.fillWidth: true
                }
                Text {
                    text: "x size"
                }

                DoubleSpinBox {
                    id: xSize
                    from: 10
                    to: 1000
                    value: 1500
                    Layout.fillWidth: true
                }
                Text {
                    text: "z size"
                }
                DoubleSpinBox {
                    id: ySize
                    from: 10
                    to: 1000
                    value: 1500
                    Layout.fillWidth: true
                }
                Text {
                    text: "height"
                }
                DoubleSpinBox {
                    id: height
                    from: 2
                    to: 500
                    value: 500
                    Layout.fillWidth: true
                }
                Text {
                    text: "milling stop"
                }
                DoubleSpinBox {
                    id: safeArea
                    from: 2
                    to: 500
                    value: 200
                    Layout.fillWidth: true
                }

                Button {
                    text: "Apply"
                    onClicked: {
                        millingMaterial.setSizes(xRes.value, yRes.value, xSize.realValue, ySize.realValue, height.realValue)
                    }
                } // button
                Button {
                    text: "Reset milled area"
                    onClicked: {
                        millingMaterial.resetHeights()
                    }
                }
            } // grid
        } //rectangle
    } // material popup
    
    header: ToolBar {
        RowLayout{
            anchors.fill:parent
            ToolButton {
                text: "Load path"
                enabled: !millingManager.running
                onClicked: openFileDialog.open()
            }
            ToolButton {
                text: "Material options"
                enabled: !millingManager.running
                onClicked: {
                    materialConfigurationPopup.open()
                }
            }
            ToolButton {
                text: "Tools"
                enabled: !millingManager.running
                onClicked: {
                    toolConfigurationPopup.open()
                }
            } // tools button

            ToolSeparator{}
            ToolButton {
                text: "Run"
                enabled: pathManager.valid && !millingManager.active
                onClicked: millingManager.Run()
            }
            ToolButton {
                text: "Pause"
                enabled: millingManager.active
                onClicked: millingManager.Pause()
            }
            ToolButton {
                text: "Stop"
                enabled: millingManager.running
                onClicked: millingManager.Stop()
            }
            ToolButton {
                text:"FF"
                enabled: millingManager.running
                onClicked: millingManager.RunToEnd()
            }
            Text {
                text: "Progress: "+millingManager.progress.toFixed(2)
                visible: millingManager.running
            }
            Text {
                text: "Milling error!"
                visible: millingManager.error
                color: "red"
            }
            ToolSeparator{}

            Item {
                Layout.fillWidth:true
            }
            ToolSeparator{}

            CheckBox {
                Layout.alignment: Qt.AlignRight
                id: renderMaterialBox
                text: "Render material"
                checked: true
            }
            CheckBox {
                Layout.alignment: Qt.AlignRight
                id: renderPathsBox
                text: "Render paths"
                checked: true
            }
        } // row
    } // header toolbar

    Item {
        id: leftPanel
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: 250
        Rectangle {
            id: header
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            radius: 2
            color: "lightgray"
            border {
                width: 2
                color: "white"
            }
            height: 40

            Text {
                text: "Paths"
                width: parent.width
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 24
                font.bold: true
                color: "gray"
            }
        }
        Item{
            anchors {
                top: header.bottom
                left:parent.left
                right:parent.right
                bottom:parent.bottom
            }

        Component {
            id: toolViewDelegate
            Rectangle{
                anchors {
                    left: parent.left
                    right: parent.right
                }
                border {
                    width: 2
                    color: "gray"
                }
                color: "lightgray"
                radius: 2
                height: 80

                Column{
                    id: column
                    width:200
                    anchors {
                        top: parent.top
                        left:parent.left
                        bottom:parent.bottom
                        margins: 3
                    }
                    Text{
                        text: {
                            var txt = file.split('/');
                            var name = txt[txt.length-1]
                            return"file: "+ name
                        }
                    }
                    Text{text: "tool radius: "+tool.toolRadius}
                    Text{text: "tool type: "+tool.type}
                    Text{text: "length: "+pathLength.toFixed(2)}
                }
                Button {
                    enabled: !millingManager.running
                    anchors {
                        top:parent.top
                        left: column.right
                        right: parent.right
                        bottom: parent.bottom
                    }
                    text: "R"
                    onClicked: {
                        pathManager.RemovePath(index)
                    }
                    ToolTip.text: "Remove this path from program"
                    ToolTip.visible: hovered

                }
            }
        } // toolViewDelegate

        ListView{
            anchors.fill: parent
            model: pathManager.paths
            delegate: toolViewDelegate
            spacing: 2
        } // listView
    } }

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
                cool: "lightgreen"
            }
            CuboidMesh {
                id: tableMesh
                xExtent: 300
                yExtent: 1
                zExtent: 300
            }

            SphereMesh {
                id: toolMesh
                radius: 2
            }

            Transform {
                id: toolTranslation
                translation: {
                    var p = millingManager.toolpos
                    return Qt.vector3d(p.x, p.y+2, p.z);
                }
            }

            Entity {
                id: toolEntity
                enabled: millingManager.running
                components: [toolMesh, tableMaterial, toolTranslation]
            }

            Entity {
                id: table
                components: [tableMesh, tableMaterial]
            }

            Catu.Material {
                id: millingMaterial
                enabled: renderMaterialBox.checked
            }

            Catu.PathRender {
                id: renderedPaths
                paths: pathManager
                enabled: renderPathsBox.checked
            }

        } // root Entity
    } // Scene3d
}
