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
        Q_PROPERTY(bool slerp WRITE setSlerp);
        Q_PROPERTY(double animationTime READ getAnimationTime WRITE setAnimationTime NOTIFY animationTimeChanged);

        Q_PROPERTY(double simpleR2 READ getSimpleR2 NOTIFY simpleStateChanged);
        Q_PROPERTY(double simpleQ1 READ getSimpleQ1 NOTIFY simpleStateChanged);
        Q_PROPERTY(double simpleQ2 READ getSimpleQ2 NOTIFY simpleStateChanged);
        Q_PROPERTY(double simpleQ3 READ getSimpleQ3 NOTIFY simpleStateChanged);
        Q_PROPERTY(double simpleQ4 READ getSimpleQ4 NOTIFY simpleStateChanged);
        Q_PROPERTY(double simpleQ5 READ getSimpleQ5 NOTIFY simpleStateChanged);

        Q_PROPERTY(double advancedR2 READ getAdvancedR2 NOTIFY advancedStateChanged);
        Q_PROPERTY(double advancedQ1 READ getAdvancedQ1 NOTIFY advancedStateChanged);
        Q_PROPERTY(double advancedQ2 READ getAdvancedQ2 NOTIFY advancedStateChanged);
        Q_PROPERTY(double advancedQ3 READ getAdvancedQ3 NOTIFY advancedStateChanged);
        Q_PROPERTY(double advancedQ4 READ getAdvancedQ4 NOTIFY advancedStateChanged);
        Q_PROPERTY(double advancedQ5 READ getAdvancedQ5 NOTIFY advancedStateChanged);


        Q_PROPERTY(QVector3D startPos READ getStartPos WRITE setStartPos NOTIFY startPosChanged);
        Q_PROPERTY(QVector3D endPos READ getEndPos WRITE setEndPos NOTIFY endPosChanged);

        Q_PROPERTY(QVector3D startEuler READ getStartEuler WRITE setStartEuler NOTIFY startEulerChanged);
        Q_PROPERTY(QVector3D endEuler READ getEndEuler WRITE setEndEuler NOTIFY endEulerChanged);

        Q_PROPERTY(QQuaternion startQuat READ getStartQuat WRITE setStartQuat NOTIFY startQuatChanged);
        Q_PROPERTY(QQuaternion endQuat READ getEndQuat WRITE setEndQuat NOTIFY endQuatChanged);

        bool getRunning() const;
        double getAnimationTime() const;

        double getSimpleQ1() const;
        double getSimpleR2() const;
        double getSimpleQ2() const;
        double getSimpleQ3() const;
        double getSimpleQ4() const;
        double getSimpleQ5() const;

        double getAdvancedQ1() const;
        double getAdvancedR2() const;
        double getAdvancedQ2() const;
        double getAdvancedQ3() const;
        double getAdvancedQ4() const;
        double getAdvancedQ5() const;

        QVector3D getStartPos() const;
        QVector3D getEndPos() const;

        QVector3D getStartEuler() const;
        QVector3D getEndEuler() const;

        QQuaternion getStartQuat() const;
        QQuaternion getEndQuat() const;

        void setSlerp(bool val);
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

        void simpleStateChanged();
        void advancedStateChanged();

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
        struct Data{
            Data(): q1(0), q2(0), q3(0), q4(0),q5(0),r2(1){}
            double q1,q2,q3,q4,q5,r2;
        };


        void computeCurrentData();
        void computeSimpleData(double at);
        void computeAdvancedData(double at);

        Data computeIK(QVector3D pos, QQuaternion rot, bool preserve, const Data& prev);

        void tick();
        double time;

        Data start,end,current;
        Data currentAdvanced;

        QVector3D startPos;
        QVector3D endPos;

        // euler angles
        QVector3D startEuler;
        QVector3D endEuler;

        //quats
        QQuaternion startQuat;
        QQuaternion endQuat;

        double r1,r3,r4;


        bool running = false;
        bool slerp = false;
        double animationTime;
        std::unique_ptr<QTimer> timer;
};
