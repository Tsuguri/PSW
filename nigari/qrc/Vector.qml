import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Entity {
    id: vector

    property color color: "#32BC25"
    property var simulator

    property vector3d coordinates: Qt.vector3d(0, 10, 0)
    readonly property real length: coordinates.length()

    Transform {
        id: rotationTransform
        rotation: vector.simulator.fromToQuaternion(Qt.vector3d(0, 1, 0), vector.coordinates)
    }

    PhongMaterial {
        id: material
        ambient: vector.color
    }

    CylinderMesh {
        id: shaftMesh
        length: 0.8 * vector.length
        radius: 0.02 * vector.length
    }

    ConeMesh {
        id: tipMesh
        length: 0.2 * vector.length
        topRadius: 0.0
        bottomRadius: 0.05 * vector.length
    }

    Transform {
        id: shaftTransform
        translation: Qt.vector3d(0, shaftMesh.length / 2.0, 0)
    }

    Transform {
        id: tipTransform
        translation: Qt.vector3d(0, shaftMesh.length + tipMesh.length / 2.0, 0)
    }

    Entity {
        id: arrow

        Entity {
            id: shaft
            components: [ shaftMesh, material, shaftTransform ]
        }

        Entity {
            id: tip
            components: [ tipMesh, material, tipTransform ]
        }

        components: [ rotationTransform ]
    }

}