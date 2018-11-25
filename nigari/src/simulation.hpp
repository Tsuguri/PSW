#pragma once

#include <memory>

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include <QQuaternion>
#include <QGenericMatrix>

using Mat = QGenericMatrix<3,3,double>;

struct State {
    QVector3D angleVelocity;
    QQuaternion rotation;
};

class Simulation : public QObject {

    Q_OBJECT
    public:
        explicit Simulation(QObject* parent = nullptr);
        Q_PROPERTY(QQuaternion rotation READ getRotation NOTIFY rotationChanged);

        Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged);
        Q_PROPERTY(bool gravity READ getGravity WRITE setGravity NOTIFY gravityChanged);
        Q_PROPERTY(double cubeSide WRITE setCubeSide READ getCubeSide NOTIFY cubeChanged);
        Q_PROPERTY(double ro WRITE setRo READ getRo NOTIFY roChanged);
        Q_PROPERTY(double angleVelocity WRITE setAngleVel READ getAngleVel NOTIFY angleVelChanged);
        Q_PROPERTY(double diagonalAngle WRITE setDiagonalAngle READ getDiagonalAngle NOTIFY diagonalAngleChanged);

        QQuaternion getRotation() const;
        double getCubeSide() const;
        bool getRunning() const;
        double getRo() const;
        double getDiagonalAngle() const;
        double getAngleVel() const;
        bool getGravity() const;

        void setCubeSide(double len);
        void setRo(double ro);
        void setDiagonalAngle(double val);
        void setAngleVel(double val);
        void setGravity(bool val);

signals:
        void rotationChanged();
        void cubeChanged();
        void runningChanged();
        void roChanged();
        void diagonalAngleChanged();
        void angleVelChanged();
        void gravityChanged();

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
    void computeInertia();
    void computeStartingPos();

    double Mass() const;

    double cube = 5.0f;
    bool running = false;
    double ro = 1.0f;
  std::unique_ptr<QTimer> timer;
  std::unique_ptr<QElapsedTimer> elapsed;
  quint64 elapsedNotUsed;

  QQuaternion currentRotation;
  QQuaternion constantChange;
  double time;
  double diagonalAngle;
  double angleVelocity;
  bool gravity;
  State prev, prevPrev;

  Mat inertiaTensor;
  Mat invertedInertiaTensor;
};
