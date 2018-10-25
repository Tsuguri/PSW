import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
    id: root

    width: 140
    height: 40

    property real from: 0.0
    property real to: 100.0
    property int decimals: 1

    property alias value: spinbox.value

    readonly property real realValue: value / multiplier

    readonly property real multiplier: Math.pow(10, decimals)

    readonly property bool valid: realValue >= from && realValue <= to

    SpinBox {
        id: spinbox
        anchors.fill: parent

        editable: true

        from: Math.floor(root.from * root.multiplier)
        value: 0
        to: Math.ceil(root.to * root.multiplier)
        stepSize: root.multiplier

        validator: DoubleValidator {
            bottom: Math.min(spinbox.from, spinbox.to)
            top:  Math.max(spinbox.from, spinbox.to)
        }

        textFromValue: function(value, locale) {
            return Number(value / root.multiplier).toLocaleString(locale, 'f', root.decimals)
        }

        valueFromText: function(text, locale) {
            return Number.fromLocaleString(locale, text) * root.multiplier
        }
    }
}
