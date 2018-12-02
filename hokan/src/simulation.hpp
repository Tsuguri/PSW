#pragma once

#include <memory>

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include <QQuaternion>
#include <QGenericMatrix>

using Mat = QGenericMatrix<3,3, float>;

class Simulation : public QObject {

    Q_OBJECT
    public:
        explicit Simulation(QObject* parent = nullptr);

        Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged);
        Q_PROPERTY(double animationTime READ getAnimationTime WRITE setAnimationTime NOTIFY animationTimeChanged);

        Q_PROPERTY(QVector3D currentPos READ getCurrentPos NOTIFY currentStateChanged);

        Q_PROPERTY(QQuaternion currentQuat READ getCurrentQuat NOTIFY currentStateChanged);
        Q_PROPERTY(QVector3D currentEuler READ getCurrentEuler NOTIFY currentStateChanged);


        Q_PROPERTY(QVector3D startPos READ getStartPos WRITE setStartPos NOTIFY startPosChanged);
        Q_PROPERTY(QVector3D endPos READ getEndPos WRITE setEndPos NOTIFY endPosChanged);

        Q_PROPERTY(QVector3D startEuler READ getStartEuler WRITE setStartEuler NOTIFY startEulerChanged);
        Q_PROPERTY(QVector3D endEuler READ getEndEuler WRITE setEndEuler NOTIFY endEulerChanged);

        Q_PROPERTY(QQuaternion startQuat READ getStartQuat WRITE setStartQuat NOTIFY startQuatChanged);
        Q_PROPERTY(QQuaternion endQuat READ getEndQuat WRITE setEndQuat NOTIFY endQuatChanged);

        bool getRunning() const;
        double getAnimationTime() const;
        QVector3D getCurrentPos() const;
        QVector3D getCurrentEuler() const;
        QQuaternion getCurrentQuat() const;

        QVector3D getStartPos() const;
        QVector3D getEndPos() const;

        QVector3D getStartEuler() const;
        QVector3D getEndEuler() const;

        QQuaternion getStartQuat() const;
        QQuaternion getEndQuat() const;

        void setAnimationTime(double val);
        void setStartPos(const QVector3D& val);
        void setEndPos(const QVector3D& val);
        void setStartEuler(const QVector3D& val);
        void setEndEuler(const QVector3D& val);
        void setStartQuat(const QQuaternion& quat);
        void setEndQuat(const QQuaternion& quat);
signals:
        void runningChanged();
        void animationTimeChanged();
        void currentStateChanged();
        void endPosChanged();
        void startPosChanged();
        void startEulerChanged();
        void endEulerChanged();
        void startQuatChanged();
        void endQuatChanged();
    public:
        Q_INVOKABLE void toggleRun();
        Q_INVOKABLE void reset();

    private:
        void computeCurrentData();

        void tick();
        double time;

        QVector3D currentPos;
        QVector3D currentEuler;
        QQuaternion currentQuat;

        QVector3D startPos;
        QVector3D endPos;

        // euler angles
        QVector3D startEuler;
        QVector3D endEuler;

        //quats
        QQuaternion startQuat;
        QQuaternion endQuat;


        bool running = false;
        double animationTime;
        std::unique_ptr<QTimer> timer;
};
