#pragma once

#include <memory>

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include <QQuaternion>


class Simulation : public QObject {

    Q_OBJECT
    public:
        explicit Simulation(QObject* parent = nullptr);
        Q_PROPERTY(QQuaternion rotation READ getRotation NOTIFY rotationChanged);

        Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged);
        Q_PROPERTY(float cubeSide WRITE setCubeSide READ getCubeSide NOTIFY cubeChanged);
        Q_PROPERTY(float ro WRITE setRo READ getRo NOTIFY roChanged);

        QQuaternion getRotation() const;
        float getCubeSide() const;
        bool getRunning() const;
        float getRo() const;

        void setCubeSide(float len);
        void setRo(float ro);


signals:
        void rotationChanged();
        void cubeChanged();
        void runningChanged();
        void roChanged();

    public:

    Q_INVOKABLE void toggleRun();
    Q_INVOKABLE void reset();

    Q_INVOKABLE QQuaternion quatFromTo(const QVector3D& from, const QVector3D& to) const{
        return QQuaternion::rotationTo(from, to);
    }
    Q_INVOKABLE QVector3D quatTimesVec(const QQuaternion& quat, const QVector3D& vec) const{
        return quat*vec;
    }

    private:

    void tick();

    float cube = 5.0f;
    bool running = false;
    float ro = 1.0f;
  std::unique_ptr<QTimer> timer;
  std::unique_ptr<QElapsedTimer> elapsed;
  quint64 elapsedNotUsed;

  QQuaternion currentRotation;
  QQuaternion constantChange;
  float time;
};
