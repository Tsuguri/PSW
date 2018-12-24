#include "simulation.hpp"

#include <iostream>
#include <cmath>

#include "QQuaternionUtils.hpp"

Simulation::Simulation(QObject* parent) : QObject(parent), running(false), timer(new QTimer(this)), 
simpleQ1(0), simpleQ2(0), simpleQ3(0), simpleQ4(0), simpleQ5(0), simpleR2(1),
advancedQ1(0), advancedQ2(0), advancedQ3(0), advancedQ4(0), advancedQ5(0), advancedR2(1)
{

  QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
  timer->setInterval(static_cast<int>(33));
  time = 0.0;
}

bool Simulation::getRunning() const {
    return running;
}

double Simulation::getAnimationTime() const {
    return animationTime;
}

double Simulation::getSimpleQ1() const {
    return simpleQ1;
}
double Simulation::getSimpleQ2() const {
    return simpleQ2;
}
double Simulation::getSimpleQ3() const {
    return simpleQ3;
}
double Simulation::getSimpleQ4() const {
    return simpleQ4;
}
double Simulation::getSimpleQ5() const {
    return simpleQ5;
}

double Simulation::getSimpleR2() const {
    return simpleR2;
}


double Simulation::getAdvancedQ1() const {
    return simpleQ1;
}
double Simulation::getAdvancedQ2() const {
    return advancedQ2;
}
double Simulation::getAdvancedQ3() const {
    return advancedQ3;
}
double Simulation::getAdvancedQ4() const {
    return advancedQ4;
}
double Simulation::getAdvancedQ5() const {
    return advancedQ5;
}

double Simulation::getAdvancedR2() const {
    return advancedR2;
}


void Simulation::setAnimationTime(double val) {
    animationTime=val;
    emit animationTimeChanged();
}

QVector3D Simulation::getStartPos() const {
    return startPos;
}

QVector3D Simulation::getEndPos() const {
    return endPos;
}


QVector3D Simulation::getStartEuler() const {
    return startEuler;
}

QVector3D Simulation::getEndEuler() const {
    return endEuler;
}

QQuaternion Simulation::getStartQuat() const {
    return startQuat;
}

QQuaternion Simulation::getEndQuat() const {
    return endQuat;
}

void Simulation::setSlerp(bool val){
    slerp = val;
}

void Simulation::setStartPos(const QVector3D& val) {
    startPos=val;
    emit startPosChanged();
}

void Simulation::setEndPos(const QVector3D& val){
    endPos=val;
    emit endPosChanged();
}

void Simulation::setStartEuler(const QVector3D& val) {
    startEuler = val;
    startQuat = QQuaternion::fromEulerAngles(startEuler);
    emit startEulerChanged();
    emit startQuatChanged();
}

void Simulation::setEndEuler(const QVector3D& val) {
    endEuler = val;
    endQuat = QQuaternion::fromEulerAngles(endEuler);
    emit endEulerChanged();
    emit endQuatChanged();
}

void Simulation::setStartQuat(const QQuaternion& quat) {
    startQuat = quat.normalized();
    startEuler = startQuat.toEulerAngles();
    emit startQuatChanged();
    emit startEulerChanged();
}

void Simulation::setEndQuat(const QQuaternion& quat) {
    endQuat = quat.normalized();
    endEuler = endQuat.toEulerAngles();
    emit endQuatChanged();
    emit endEulerChanged();
}

void Simulation::toggleRun() {
    if(running){
        timer->stop();
    } else {
        timer->start();
    }

    running=!running;
    emit runningChanged();
}

void Simulation::reset() {
    timer->stop();
    time = 0.0;
    running = false;
    emit runningChanged();
    computeCurrentData();

}

void Simulation::tick() {
    time += 0.033;
    bool end = false;
    if(time>animationTime){
        time = animationTime;
        end = true;
    }
    computeCurrentData();
    if(end){
        toggleRun();
        time = 0.0;
    }
}

float interpolateAngle(float from, float to, float at){
    while(from > 360) {
        from -=360;
    }
    while(to> 360) {
        to-=360;
    }
    auto div = to-from;
    if(std::abs(div)>180){
        div = div-(div>0 ? 1 : -1)*360;
    }

    auto change = div*at;

    auto result = from+change;
    if(result<0){
        result +=360;
    }
    if(result>360){
        result -=360;
    }

    return result;
}

QVector3D interpolateEuler(double at, const QVector3D& from, const QVector3D to){
    auto x = interpolateAngle(from.x(), to.x(), at);
    auto y = interpolateAngle(from.y(), to.y(), at);
    auto z = interpolateAngle(from.z(), to.z(), at);

    return QVector3D(x,y,z);
}

float dot(const QQuaternion& q1, const QQuaternion& q2) {
    return q1.x()*q2.x() + q1.y()*q2.y() + q1.z()*q2.z() + q1.scalar()*q2.scalar();
}

QQuaternion lerp(double at, const QQuaternion& from, const QQuaternion& to){
    auto to2 = to;
    if (dot(from, to)<0){
        to2 = -to;
    }
    return (from*(1-at) + to2*at).normalized();
}

QQuaternion slerpo(double at, const QQuaternion& from, const QQuaternion& to){
    auto to2 = to;
    auto dotto = dot(from, to);
    if (dotto<0){
        to2 = -to;
        dotto = -dotto;
    }
    if(dotto>0.999995) {
        return lerp(at,from,to);
    }
    auto theta0 = std::acos(dotto);
    auto theta = at*theta0;
    auto sinTheta = std::sin(theta);
    auto sinTheta0 = std::sin(theta0);

    auto s0 = cos(theta) - dotto * sinTheta /sinTheta0;
    auto s1 = sinTheta / sinTheta0;

    return (s0*from + s1*to2).normalized();
}

void Simulation::computeCurrentData(){
    double at = time/animationTime;
    double mat = 1-at;

    simpleQ1 = 360* at;
    simpleQ2 = 360* at;
    simpleQ3 = 360* at;
    simpleQ4 = 360* at;
    simpleQ5 = 360* at;

    emit simpleStateChanged();
}
