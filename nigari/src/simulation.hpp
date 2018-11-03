#pragma once
#include <QObject>

#include <QQuaternion>


class Simulation : public QObject {

    Q_OBJECT
    public:
        Q_PROPERTY(QQuaternion rotation READ getRotation, NOTIFY rotationChanged);

        QQuaternion getRotation() const;

signals:
        void rotationChanged();

    Q_INVOKABLE QQuaternion quatFromTo(const QVector3D& from, const QVector3D& to){
        return QQuaternion::rotationTo(from, to);
    }
};
