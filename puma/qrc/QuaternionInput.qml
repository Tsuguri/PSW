import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

    Grid{
        id: root
        property alias xVal:xField.value
        property alias yVal:yField.value
        property alias zVal:zField.value
        property alias wVal:wField.value

        property alias xT: xField.text
        property alias yT: yField.text
        property alias zT: zField.text
        property alias wT: wField.text

        property quaternion value: Qt.quaternion(wVal,xVal, yVal, zVal)
        property string text : "Quaternion "
        columns: 5

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
        DoubleTextField{
            width: 60
            id: wField
        }
    }
