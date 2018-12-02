import QtQuick 2.7
import QtQuick.Controls 2.2

TextField {
    id: textField

    text: "0.0"
    selectByMouse: true

    readonly property real value: Number(text)

    property alias minValue: validator.bottom
    property alias maxValue: validator.top
    property alias decimals: validator.decimals

    validator: DoubleValidator {
        id: validator
        bottom: -1000.0
        top: 1000.0
        notation: DoubleValidator.StandardNotation
        decimals: 3
        locale: "C"
    }

    background: Rectangle {
        border.width: 1
        color: textField.acceptableInput ? "white" : "crimson"
    }
}
