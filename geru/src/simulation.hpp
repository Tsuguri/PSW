#pragma once


#include <QObject>
#include <QVector3D>

class Simulation : public QObject {
    Q_OBJECT

    public:
        explicit Simulation(QObject* parent = nullptr);

        Q_PROPERTY(QVector3D framePos READ getFramePos WRITE setFramePos NOTIFY framePosChanged);
        Q_PROPERTY(QVector3D frameRot READ getFrameRot WRITE setFrameRot NOTIFY frameRotChanged);


        void setFramePos(QVector3D val);
        void setFrameRot(QVector3D val);

        QVector3D getFramePos() const;
        QVector3D getFrameRot() const;
signals:
        void framePosChanged();
        void frameRotChanged();


    private:
        QVector3D framePosition;
        QVector3D frameRotation;
};
