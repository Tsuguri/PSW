import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

TextField {
    id: root
    property real min: 0.0
    property real max: 1.0
    property int decimals: 2.0
    property alias valid: root.acceptableInput

    selectByMouse: true
    text: "0.0"
    background: Rectangle {
        border.width: 1
        border.color: valid? "lightgray" : "red"
    }
    validator: DoubleValidator {
        bottom: min;
        top: max
        decimals: decimals;
        notation: DoubleValidator.StandardNotation;
    }
}
