import Qt3D.Core 2.0
import Qt3D.Render 2.0

Material {
    id: root

    property color ambient: Qt.rgba( 0.1, 0.1, 0.1, 1.0 )
    property color diffuse: Qt.rgba( 0.5, 0.5, 0.9, 1.0 )
    property color specular: Qt.rgba( 0.95, 0.95, 0.95, 1.0 )
    property real shininess: 150.0
	property vector4d light1Position: Qt.vector4d(0, 0, 0, 1)
	property vector4d light2Position: Qt.vector4d(0, 0, 0, 1)
	property vector3d lightIntensity: Qt.vector3d(1, 1, 1)

    property real minX: -1.0
    property real maxX: 1.0
    property real minY: -1.0
    property real maxY: 1.0
    property real minZ: -1.0
    property real maxZ: 1.0

    effect: BezierCubeEffect {}

    property Buffer bezierBuffer: Buffer {
        usage: Buffer.DynamicDraw
    }

    parameters: [
		Parameter { name: "light[0].position"; value: root.light1Position },
		Parameter { name: "light[0].intensity"; value: root.lightIntensity },
		Parameter { name: "light[1].position"; value: root.light2Position },
		Parameter { name: "light[1].intensity"; value: root.lightIntensity },
        Parameter { name: "ka"; value: Qt.vector3d(root.ambient.r, root.ambient.g, root.ambient.b) },
        Parameter { name: "kd"; value: Qt.vector3d(root.diffuse.r, root.diffuse.g, root.diffuse.b) },
        Parameter { name: "ksp"; value: Qt.vector3d(root.specular.r, root.specular.g, root.specular.b) },
        Parameter { name: "shininess"; value: root.shininess },
        Parameter { name: "bezier"; value: root.bezierBuffer },
        Parameter { name: "minX"; value: root.minX },
        Parameter { name: "maxX"; value: root.maxX },
        Parameter { name: "minY"; value: root.minY },
        Parameter { name: "maxY"; value: root.maxY },
        Parameter { name: "minZ"; value: root.minZ },
        Parameter { name: "maxZ"; value: root.maxZ }
    ]
}
