import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtCharts 2.0

ApplicationWindow {
    id: appWindow
    visible: true
    title: "Hokan"

    width: 1200
    height: 800

    minimumWidth: 1000
    minimumHeight: 600

    Shortcut {
        sequence: "Esc"
        onActivated: appWindow.close()
    }

    Scene3D {
        id: scene3d
        anchors.fill: parent
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
                            clearColor: "gray"

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
                                    }

                                }
                            }
                        }

                        Layer { id: l1 }
                        Layer { id: l2 }

                        // Event Source will be set by the Qt3DQuickWindow
                        InputSettings { id: inputSettings }

                        Entity {
                            id: cameraSet

                            Camera {
                                id: camera
                                projectionType: CameraLens.PerspectiveProjection
                                fieldOfView: 30
                                aspectRatio: scene3d.width / 2 / scene3d.height
                                nearPlane : 0.1
                                farPlane : 1000.0
                                position: Qt.vector3d( 1.0, 7.0, 7.0 )
                                upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                                viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
                            }

                            OrbitCameraController {
                                camera: camera
                                linearSpeed: 10
                                lookSpeed: 1000
                            }

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
                        Entity {
                            id: sceneRoot

                            Entity {
                                id: base
                                components: [gravityMat, baseMesh, l1, l2]
                            } // base
                            
                        } // sceneRoot
                    }// rootNode
                } // scene3d
}
