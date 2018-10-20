import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Rectangle {
    property bool running: false
    property real basePosition: 0.0
    property real springLength: 6.0

    border {
        width: 5
        color: running ? "lightgreen" : "lightblue"
    } // border

    Scene3D {
        anchors.fill: parent
        anchors.margins: 3
        focus: true
        aspects: ["input","logic","render"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Entity {
            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                nearPlane: 0.1
                farPlane: 100.0
                position: Qt.vector3d(0.0, 5.0, 30.0)
                upVector: Qt.vector3d(0.0, 1.0, 0.0)
                viewCenter: Qt.vector3d(0.0, -5.0, 0.0)
            }

            OrbitCameraController {
                camera: camera
                linearSpeed: 0
            }

            components:[
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        camera: camera
                        clearColor: "lightGray"
                    }
                },
                InputSettings{},
                DirectionalLight{
                    worldDirection: Qt.vector3d(1, -1, 1)
                },
                DirectionalLight{
                    worldDirection: Qt.vector3d(-1, -1, -1)
                }
            ]

            PhongMaterial {
                id: metalMaterial
                ambient: "#BDBDBD"
                diffuse: "#BDBDBD"
                specular: "#FAFAFA"
            }

            CylinderMesh {
                id: weightMesh
                length: 2.0
                radius: 4.0
            }

            CylinderMesh {
                id: springMesh
                length: springLength
                radius: 0.5
            }

            CuboidMesh {
                id: baseMesh
                xExtent: 8.0
                yExtent: 0.5
                zExtent: 8.0
            }

            Transform {
                id: weightTransform
                matrix: {
                    var m = Qt.matrix4x4();
                    m.translate(Qt.vector3d(0.0,basePosition - baseMesh.yExtent/2.0 - weightMesh.length/2.0 - springLength,0.0));
                    return m
                }
            }

            Transform {
                id: baseTransform
                matrix: {
                    var m = Qt.matrix4x4();
                    m.translate(Qt.vector3d(0.0,basePosition,0.0));
                    return m;
                }
            }

            Transform {
                id: springTransform
                matrix: {
                    var m = Qt.matrix4x4();
                    m.translate(Qt.vector3d(0.0,basePosition - baseMesh.yExtent/2.0 - springLength/2.0,0.0));
                    return m;
                }
            }

            Entity {
                id: base
                components: [baseMesh, metalMaterial, baseTransform]
            }

            Entity {
                id: weight
                components: [weightMesh, metalMaterial, weightTransform]
            }

            Entity {
                id: spring
                components: [springMesh, metalMaterial, springTransform]
            }


        } // root Entity
    } // Scene3d
} // main Rectangle
