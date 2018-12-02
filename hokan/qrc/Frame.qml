import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import Hoken 1.0

Entity {
    id: frame

    CylinderMesh {
        id: mesh
        radius: 0.1
        length: 1.0
    }
    PhongMaterial {
        id: matUp
        ambient: "blue"
    }

    PhongMaterial {
        id: matRight
        ambient: "red"
    }

    PhongMaterial {
        id: matForward
        ambient: "green"
    }
    Transform {
        id: tr
        translation: frame.position
        rotation: frame.rotation
    }

    property var layers: []
    property vector3d position
    property quaternion rotation

    components: [tr].concat(layers)

    Transform {
        id: upTr
        translation: Qt.vector3d(0.5,0,0)
        rotation: Quat.fromToQuaternion(Qt.vector3d(0,1,0), Qt.vector3d(1,0,0))
    }

    Transform {
        id: rightTr
        translation: Qt.vector3d(0, 0.5,0)
    }

    Transform {
        id: forwardTr
        translation: Qt.vector3d(0,0, 0.5)
        rotation: Quat.fromToQuaternion(Qt.vector3d(0,1,0), Qt.vector3d(0,0,1))
    }

    Entity {
        components: [upTr, mesh, matUp].concat(frame.layers)
    }

    Entity {
        components: [rightTr, mesh, matRight].concat(frame.layers)
    }

    Entity {
        components: [forwardTr, mesh, matForward].concat(frame.layers)
    }

}
