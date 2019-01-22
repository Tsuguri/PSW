#include "simulation.hpp"

#include <cmath>
#include <iostream>

#include "QQuaternionUtils.hpp"

Simulation::Simulation(QObject* parent)
    : QObject(parent),
      running(false),
      timer(new QTimer(this)),
      start(),
      end(),
      current(),
      currentAdvanced() {

    QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
    timer->setInterval(static_cast<int>(33));
    time = 0.0;

    r1 = 1;
    r3 = 1;
    r4 = 1;
}

bool Simulation::getRunning() const {
    return running;
}

double Simulation::getAnimationTime() const {
    return animationTime;
}

double Simulation::getSimpleQ1() const {
    return current.q1;
}
double Simulation::getSimpleQ2() const {
    return current.q2;
}
double Simulation::getSimpleQ3() const {
    return current.q3;
}
double Simulation::getSimpleQ4() const {
    return current.q4;
}
double Simulation::getSimpleQ5() const {
    return current.q5;
}

double Simulation::getSimpleR2() const {
    return current.r2;
}

double Simulation::getAdvancedQ1() const {
    return currentAdvanced.q1;
}
double Simulation::getAdvancedQ2() const {
    return currentAdvanced.q2;
}
double Simulation::getAdvancedQ3() const {
    return currentAdvanced.q3;
}
double Simulation::getAdvancedQ4() const {
    return currentAdvanced.q4;
}
double Simulation::getAdvancedQ5() const {
    return currentAdvanced.q5;
}

double Simulation::getAdvancedR2() const {
    return currentAdvanced.r2;
}

void Simulation::setAnimationTime(double val) {
    animationTime = val;
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

void Simulation::setSlerp(bool val) {
    slerp = val;
}

void Simulation::setStartPos(const QVector3D& val) {
    startPos = val;
    emit startPosChanged();
}

void Simulation::setEndPos(const QVector3D& val) {
    endPos = val;
    emit endPosChanged();
}

void Simulation::setStartEuler(const QVector3D& val) {
    startEuler = val;
    startQuat  = QQuaternion::fromEulerAngles(startEuler);
    emit startEulerChanged();
    emit startQuatChanged();
}

void Simulation::setEndEuler(const QVector3D& val) {
    endEuler = val;
    endQuat  = QQuaternion::fromEulerAngles(endEuler);
    emit endEulerChanged();
    emit endQuatChanged();
}

void Simulation::setStartQuat(const QQuaternion& quat) {
    startQuat  = quat.normalized();
    startEuler = startQuat.toEulerAngles();
    emit startQuatChanged();
    emit startEulerChanged();
}

void Simulation::setEndQuat(const QQuaternion& quat) {
    endQuat  = quat.normalized();
    endEuler = endQuat.toEulerAngles();
    emit endQuatChanged();
    emit endEulerChanged();
}

void Simulation::toggleRun() {
    if (running) {
        timer->stop();
    } else {
        if (time < 0.0000001) {
            start = computeIK(startPos, startQuat, false, start);
            end   = computeIK(endPos, endQuat, false, start);
        }
        timer->start();
    }

    running = !running;
    emit runningChanged();
}

void Simulation::reset() {
    timer->stop();
    time    = 0.0;
    running = false;
    emit runningChanged();
    computeCurrentData();
}

void Simulation::tick() {
    time += 0.033;
    bool end = false;
    if (time > animationTime) {
        time = animationTime;
        end  = true;
    }
    computeCurrentData();
    if (end) {
        toggleRun();
        time = 0.0;
    }
}

float dot(const QQuaternion& q1, const QQuaternion& q2) {
    return q1.x() * q2.x() + q1.y() * q2.y() + q1.z() * q2.z() + q1.scalar() * q2.scalar();
}

QQuaternion lerp(double at, const QQuaternion& from, const QQuaternion& to) {
    auto to2 = to;
    if (dot(from, to) < 0) {
        to2 = -to;
    }
    return (from * (1 - at) + to2 * at).normalized();
}

QQuaternion slerpo(double at, const QQuaternion& from, const QQuaternion& to) {
    if (at < 0) at = 0;
    if (at > 1) at = 1;
    auto to2   = to;
    auto dotto = dot(from, to);
    if (dotto < 0) {
        to2   = -to;
        dotto = -dotto;
    }
    if (dotto > 0.999995) {
        return lerp(at, from, to);
    }
    auto theta0    = std::acos(dotto);
    auto theta     = at * theta0;
    auto sinTheta  = std::sin(theta);
    auto sinTheta0 = std::sin(theta0);

    auto s0 = cos(theta) - dotto * sinTheta / sinTheta0;
    auto s1 = sinTheta / sinTheta0;

    return (s0 * from + s1 * to2).normalized();
}

template <typename T>
T interpolate(T one, T two, double at) {
    if (at < 0) at = 0;
    if (at > 1) at = 1;
    return one * (1 - at) + two * at;
}

float interpolateAngle(float from, float to, float at) {
    while (from > 360) {
        from -= 360;
    }
    while (to > 360) {
        to -= 360;
    }
    auto div = to - from;
    if (std::abs(div) > 180) {
        div = div - (div > 0 ? 1 : -1) * 360;
    }

    auto change = div * at;

    auto result = from + change;
    if (result < 0) {
        result += 360;
    }
    if (result > 360) {
        result -= 360;
    }

    return result;
}

void Simulation::computeSimpleData(double at) {
    current.q1 = interpolateAngle(start.q1, end.q1, at);
    current.q2 = interpolateAngle(start.q2, end.q2, at);
    current.q3 = interpolateAngle(start.q3, end.q3, at);
    current.q4 = interpolateAngle(start.q4, end.q4, at);
    current.q5 = interpolateAngle(start.q5, end.q5, at);
    current.r2 = interpolate(start.r2, end.r2, at);
}

void Simulation::computeAdvancedData(double at) {
    auto pos        = interpolate(startPos, endPos, at);
    auto rot        = slerpo(at, startQuat, endQuat);
    currentAdvanced = computeIK(pos, rot, true, currentAdvanced);
}

float angle(const QVector3D& v, const QVector3D& w) {
    auto cross = QVector3D::crossProduct(v, w);
    auto angle = std::atan2(cross.length(), QVector3D::dotProduct(v, w));
    return angle;
}

float angle(const QVector3D& v, const QVector3D& w, const QVector3D& n024) {
    auto cross = QVector3D::crossProduct(v, w);
    auto angle = std::atan2(cross.length(), QVector3D::dotProduct(v, w));
    if (QVector3D::dotProduct(cross, n024) < 0) angle = -angle;
    return angle;
}

QVector3D Simulation::getTest() const {
    return test;
}

Simulation::Data Simulation::computeIK(QVector3D pos,
                                       QQuaternion rot,
                                       bool preserve,
                                       const Simulation::Data& prev) {

    QVector3D p1(0.0, 0.0, 0.0);
    QVector3D p2 = p1 + r1 * QVector3D(0, 1, 0);

    QVector3D p5(pos);

    // wzdłóż blue
    QVector3D p4 = p5 - r4 * rot.rotatedVector(QVector3D(1, 0, 0));
    test         = p4;
    emit testChanged();

    QVector3D n024 = QVector3D::crossProduct(p4 - p1, p2 - p1);

    if (n024.length() < 0.0001f) {
        n024 = QVector3D(0, 0, 1);
    }
    n024 = n024.normalized();
    QVector3D z4 =
        QVector3D::crossProduct(rot.rotatedVector(QVector3D(1, 0, 0)), n024).normalized();
    if (z4.length() < 0.0001f) {
        z4 = (p2 - p4).normalized();
    }
    QVector3D p3    = p4 + r3 * z4;
    QVector3D p3alt = p4 - r3 * z4;
    auto rad2Deg    = [](double r) { return r / M_PI * 180.0; };
    if (preserve) {
        if (prevQ3) {

            if ((p3alt - *prevQ3).lengthSquared() < (p3 - *prevQ3).lengthSquared()) {
                p3 = p3alt;
            }
        }
        prevQ3 = p3;
    }

    auto d = Data();

    n024 = QVector3D::crossProduct(p4 - p1, p2 - p1);
    d.q1   = -90 - rad2Deg(std::atan2(p4.z(), p4.x()));
    d.q2   = rad2Deg(angle(p2 - p1, p3 - p2, n024));
    d.r2   = (p3 - p2).length();
    d.q3   = rad2Deg(angle(p3 - p2, p4 - p3, n024));
    d.q4   = 90 - rad2Deg(angle(n024, rot.rotatedVector(QVector3D(1, 0, 0)), p3 - p4));
    d.q5   = rad2Deg(angle(
        p3 - p4, rot.rotatedVector(QVector3D(0, 1, 0)), rot.rotatedVector(QVector3D(1, 0, 0))));

    return d;
}

void Simulation::computeCurrentData() {
    double at = time / animationTime;

    computeSimpleData(at);
    computeAdvancedData(at);
    emit simpleStateChanged();
    emit advancedStateChanged();
}
