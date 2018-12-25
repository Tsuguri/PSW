import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Entity {
    id: puma

    property real r1: 1.0
    property real r2: 2.0// sterowane przez kinematyke
    property real r3: 1.0
    property real r4: 1.0
    property real r5: 1.0
    property real q1: 0.0
    property real q2: 0.0
    property real q3: 0.0
    property real q4: 0.0
    property real q5: 0.0

    CylinderMesh {
        id: r1mesh
        radius: 0.1
        length: puma.r1
    }

    CylinderMesh {
        id: r2mesh
        radius: 0.1
        length: puma.r2
    }

    CylinderMesh {
        id: r3mesh
        radius: 0.1
        length: puma.r3
    }

    CylinderMesh {
        id: r4mesh
        radius: 0.1
        length: puma.r4
    }

    CylinderMesh{
        id: jointMesh
        radius: 0.15
        length: 0.3
    }

    CuboidMesh{
        id: toolMesh
        xExtent: 0.3
        yExtent: 0.3
        zExtent: 0.3
    }

    CuboidMesh{
        id: baseMesh
        xExtent: 5.0
        yExtent: 5.0
        zExtent: 0.1
    }

    PhongMaterial {
        id: matWhite
        ambient: "gray"
    }

    PhongMaterial {
        id: matBlue
        ambient: "blue"
    }

    property var layers: []

    Transform {
        id: tr
    }
    components: [tr].concat(layers)

    Transform {
        id: r1Transform
        translation: Qt.vector3d(0,puma.r1/2,0)
        rotationY: puma.q1
    }

    Transform{
        id: joint1Transform
        translation: Qt.vector3d(0, puma.r1/2,0)
        rotationX: puma.q2
    }

    Transform{
        id: joint2Transform
        translation: Qt.vector3d(0, puma.r2/2,0)
        rotationX: puma.q3
    }

    Transform{
        id: joint3Transform
        translation: Qt.vector3d(0, puma.r3/2,0)
        rotationY: puma.q4
    }

    Transform{
        id: joint4Transform
        translation: Qt.vector3d(0, puma.r4/2,0)
        rotationY: puma.q5
    }

    Transform {
        id: r2Transform
        translation: Qt.vector3d(0, puma.r2/2, 0)
    }

    Transform {
        id: r3Transform
        translation: Qt.vector3d(0, puma.r3/2, 0)
    }

    Transform {
        id: r4Transform
        translation: Qt.vector3d(0, 0, puma.r4/2)
        rotationX: 90
    }

    Entity{
        id: base
        Transform{
            id: baseTr
            rotationX: 90
        }

        components: [baseTr, baseMesh, matBlue].concat(puma.layers)
    }

    Entity {
        components: [r1Transform, r1mesh, matWhite].concat(puma.layers)

        Entity {
            components: [joint1Transform].concat(puma.layers)

            Entity {
                Transform{
                    id: j1Tr
                    rotationZ:90
                }
                components: [ j1Tr, jointMesh, matBlue].concat(puma.layers)
            }
            Entity{
                components: [r2Transform, r2mesh, matWhite].concat(puma.layers)

                Entity{
                    components: [joint2Transform].concat(puma.layers)

                    Entity{
                        Transform{
                            id: j2Tr
                            rotationZ:90
                        }
                        components: [j2Tr, jointMesh, matBlue].concat(puma.layers)
                    }
                    Entity {
                        components: [r3Transform, r3mesh, matWhite].concat(puma.layers)

                        Entity{
                            components: [joint3Transform].concat(puma.layers)

                            Entity{
                                Transform{
                                    id: j3Tr
                                    rotationY: 90
                                }
                                components: [j3Tr, jointMesh, matBlue].concat(puma.layers)
                            }
                            Entity{
                                components: [r4Transform, r4mesh, matWhite].concat(puma.layers)

                                Entity{
                                    components: [joint4Transform].concat(puma.layers)
                                    Entity{
                                        Transform {
                                            id: toolTr
                                        }

                                        components: [toolTr, toolMesh, matBlue].concat(puma.layers)
                                        Frame{}
                                    }
                                }
                            }
                        }

                    }

                }
            }

        }
    }
}
