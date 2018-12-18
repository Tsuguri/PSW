import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

import Geru 1.0
import fhac.mascor 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    title: "Geru"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Shortcut {
        sequence: "Esc"
        onActivated: appWindow.close()
    }

    Simulation {
        id: simulator
        framePos: mouseRaycast.worldPosition
    }

    Rectangle {
        id: toolboxContainer

        width: 350

        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }

        SimulationProperties {
            id: simulationProperties

            anchors.fill: parent
            anchors.margins: 10
        }
        color: "white"
    }

    Rectangle {
        id: focusBorder

        anchors.right: parent.right
        anchors.left: toolboxContainer.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top

        border {
            width: 1
            color: scene3d.activeFocus ? "red" : "black"
        }

        Scene3D {
            id: scene3d
            focus: true
            anchors.fill: parent
            anchors.margins: 1
            aspects: ["input", "logic", "render"]
            cameraAspectRatioMode: Scene3D.UserAspectRatio

            Entity {
                id: rootNode
                components: [frameGraph, inputSettings, keyboardHandler, mouseHandler]

                CubeWalls {
                    id: cubeWalls
                    springiness: simulationProperties.wallsSpringiness
                }

                RenderSettings {
                    id: frameGraph

                    activeFrameGraph: RenderSurfaceSelector {
                        id: surfaceSelector

                        Viewport {
                            id: mainViewport
                            normalizedRect: Qt.rect(0, 0, 1, 1)

                            ClearBuffers {
                                buffers: ClearBuffers.ColorDepthBuffer
                                clearColor: "lightGray"

                                CameraSelector {
                                    camera: camera
                                }
                            }
                        }
                    }
                }

                KeyboardDevice {
                    id: keyboardSourceDevice
                }

                MouseDevice {
                    id: mouseDevice
                }

                MouseHandler {
                    id: mouseHandler
                    sourceDevice: mouseDevice

                    onPressed: {
                        scene3d.focus = true
                        if (mouse.buttons & MouseEvent.LeftButton) {

                            mouseRaycast.screenPosition = Qt.vector2d(mouse.x, mouse.y)
                        }
                    }

                    onPositionChanged: {
                        if (mouse.buttons & MouseEvent.LeftButton) {
                            mouseRaycast.screenPosition = Qt.vector2d(mouse.x, mouse.y)
                        }
                    }
                }

                Raycast {
                    id: mouseRaycast
                    viewMatrix: camera.viewMatrix
                    projectionMatrix: camera.projectionMatrix
                    viewportSize: Qt.size(scene3d.width, scene3d.height)
                    screenPosition: Qt.vector2d(scene3d.width / 2, scene3d.height / 2)
                    distance: camera.position.length()
                }

                KeyboardHandler {
                    id: keyboardHandler
                    sourceDevice: keyboardSourceDevice
                    focus: true

                    property real rotationSpeed: 2.0

                    onPressed: {

                        if (event.key === Qt.Key_A) {
                            event.accepted = true;
                            simulator.frameRot = simulator.frameRot.minus(Qt.vector3d(rotationSpeed, 0, 0))
                        } else if (event.key === Qt.Key_D) {
                            event.accepted = true;
                            simulator.frameRot = simulator.frameRot.plus(Qt.vector3d(rotationSpeed, 0, 0))
                        } else if (event.key === Qt.Key_S) {
                            event.accepted = true;
                            simulator.frameRot = simulator.frameRot.minus(Qt.vector3d(0, 0, rotationSpeed))
                        } else if (event.key === Qt.Key_W) {
                            event.accepted = true;
                            simulator.frameRot = simulator.frameRot.plus(Qt.vector3d(0, 0, rotationSpeed))
                        } else if (event.key === Qt.Key_Q) {
                            event.accepted = true;
                            simulator.frameRot = simulator.frameRot.minus(Qt.vector3d(0, rotationSpeed, 0))
                        } else if (event.key === Qt.Key_E) {
                            event.accepted = true;
                            simulator.frameRot = simulator.frameRot.plus(Qt.vector3d(0, rotationSpeed, 0))
                        } else if (event.key === Qt.Key_P) {
                            event.accepted = true;
                            particleModel.disturb();
                        }
                    }
                }

                InputSettings { id: inputSettings }

                Entity {
                    id: cameraSet

                    Camera {
                        id: camera
                        projectionType: CameraLens.PerspectiveProjection
                        aspectRatio: scene3d.width / scene3d.height
                        fieldOfView: 30
                        nearPlane : 0.1
                        farPlane : 1000.0
                        position: Qt.vector3d( 100.0, 100.0, 100.0 )
                        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
                    }

                    OrbitCameraController {
                        camera: camera
                        linearSpeed: 0
                    }

                }

                SphereMesh {
                    id: particleMesh
                    radius: 0.5
                }

                PhongMaterial {
                    id: particleMaterial
                }

                BezierCubeMaterial {
                    id: cubeMaterial
					light1Position: Qt.vector4d(cubeWalls.xMax, cubeWalls.yMax, cubeWalls.zMax, 1)
					light2Position: Qt.vector4d(cubeWalls.xMin, cubeWalls.yMin, cubeWalls.zMin, 1)
					//light1Position: Qt.vector4d(0, cubeWalls.yMax, 0, 1)
					//light2Position: Qt.vector4d(0, cubeWalls.yMax, 0, 1)
                }

                Entity {
                    id: sceneRoot

                    NodeInstantiator {

                        model: ParticleModel {
                            id: particleModel
                            mass: simulationProperties.mass
                            bezierBuffer: cubeMaterial.bezierBuffer
                        }

                        delegate: Entity {
                            id: particle

                            enabled: simulationProperties.showParticles

                            Transform {
                                id: particleTransform
                                translation: model.position
                            }

                            components: [ particleMesh, particleTransform, particleMaterial ]
                        }

                    }

                    NodeInstantiator {

                       /* model: SpringModel {
                            id: springModel
                            particleModel: particleModel
                            springConstant: simulationProperties.springConstant
                        }*/

                        delegate: Entity {
                            id: spring

                            enabled: false
                        }

                    }

                    CuboidMesh {
                        id: cubeMesh
                        xExtent: 2
                        yExtent: 2
                        zExtent: 2
                        xyMeshResolution: Qt.size(50, 50)
                        xzMeshResolution: Qt.size(50, 50)
                        yzMeshResolution: Qt.size(50, 50)
                    }

                    SphereMesh {
                        id: sphereMesh
                        radius: 1
                        rings: 50
                        slices: 50
                    }

                    CylinderMesh {
                        id: cylinderMesh
                        radius: 1
                        length: 2
                        rings: 50
                        slices: 50
                    }

                    TorusMesh {
                        id: torushMesh
                        radius: 0.7
                        minorRadius: 0.3
                        rings: 50
                        slices: 50
                    }

                    Entity {
                        id: gello
                        enabled: simulationProperties.showGello

                        property GeometryRenderer mesh: chooseMesh(simulationProperties.mesh)

                        components: [mesh, cubeMaterial]

                        function chooseMesh(name) {
                            if (name === "cube")
                                return cubeMesh
                            else if (name === "sphere")
                                return sphereMesh
                            else if (name === "cylinder")
                                return cylinderMesh
                            else if (name === "torus")
                                return torushMesh
                            else return cubeMesh
                        }
                    }

                    GeometryRenderer {
                        id: frameGeometryRenderer
                        primitiveType: GeometryRenderer.Lines
                        geometry: CuboidWireframeGeometry {
                            xExtent: particleModel.extent
                            yExtent: particleModel.extent
                            zExtent: particleModel.extent
                        }
                    }

                    Entity {
                        id: frame
                        enabled: simulationProperties.showFrame
                        Transform {
                            id: frameTransform
                            translation: simulator.framePos
                            rotationX: simulator.frameRot.x
                            rotationY: simulator.frameRot.y
                            rotationZ: simulator.frameRot.z
                            scale3D: Qt.vector3d(1.05, 1.05, 1.05)
                        }
                        components: [frameGeometryRenderer, frameTransform, particleMaterial]
                    }

                    GeometryRenderer {
                        id: wallsGeometryRenderer
                        primitiveType: GeometryRenderer.Lines
                        geometry: CuboidWireframeGeometry {
                            xExtent: cubeWalls.xMax - cubeWalls.xMin
                            yExtent: cubeWalls.yMax - cubeWalls.xMin
                            zExtent: cubeWalls.zMax - cubeWalls.zMin
                        }
                    }


                    Entity {
                        id: walls
                        enabled: simulationProperties.showWalls
                        components: [wallsGeometryRenderer, particleMaterial]
                    }

                    GeometryRenderer {
                        id: bezierFrameGeometryRenderer
                        primitiveType: GeometryRenderer.Lines
                        geometry: BezierFrameGeometry {
                            particleModel: particleModel
                        }
                    }

                    Entity {
                        id: springs
                        enabled: simulationProperties.showSprings
                        components: [bezierFrameGeometryRenderer, particleMaterial]
                    }


                }

            }

        }

    }

}
