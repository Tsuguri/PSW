#pragma once

#include <QObject>
#include <QQuaternion>

#include <QQmlEngine>
#include <QJSEngine>

class QQuaternionUtils : public QObject {
    Q_OBJECT
public:
    explicit QQuaternionUtils(QObject *parent = nullptr);

    Q_INVOKABLE QQuaternion normalizedQuaternion(const QQuaternion &q) {
        return q.normalized();
    }

    Q_INVOKABLE QQuaternion fromEulerAngles(const QVector3D &angles) {
        return QQuaternion::fromEulerAngles(angles);
    }

    Q_INVOKABLE QVector3D toEulerAngles(const QQuaternion &q) {
        return q.toEulerAngles();
    }

    Q_INVOKABLE QQuaternion fromToQuaternion(const QVector3D &from, const QVector3D &to) {
        return QQuaternion::rotationTo(from, to);
    }

    Q_INVOKABLE QVector3D rotateVector(const QQuaternion &quaternion, const QVector3D &vec) {
        return quaternion.rotatedVector(vec);
    }
};

QObject *QQuaternionUtilsSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

