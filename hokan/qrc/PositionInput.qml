import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

    Grid{
        id: root
        property alias xVal:xField.value
        property alias yVal:yField.value
        property alias zVal:zField.value


        property alias xT: xField.text
        property alias yT: yField.text
        property alias zT: zField.text


        property vector3d value: Qt.vector3d(xVal, yVal, zVal)
        property string text : "Position "
        columns: 4

        Text {
            text: root.text
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            height: 30
        }

        DoubleTextField{
            width: 60
            id: xField
        }
        DoubleTextField{
            width: 60
            id: yField
        }
        DoubleTextField{
            width: 60
            id: zField
        }
    }
