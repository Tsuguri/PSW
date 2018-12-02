#include "simulation.hpp"

#include <iostream>

#include "QQuaternionUtils.hpp"

Simulation::Simulation(QObject* parent) : QObject(parent), running(false), timer(new QTimer(this)) {

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

void Simulation::setAnimationTime(double val) {
    animationTime=val;
    emit animationTimeChanged();
}
QVector3D Simulation::getCurrentPos() const {
    return currentPos;
}

QQuaternion Simulation::getCurrentQuat() const {
    return currentQuat;
}

QVector3D Simulation::getCurrentEuler() const {
    return currentEuler;
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

void Simulation::setStartPos(const QVector3D& val) {
    startPos=val;
    std::cout<<"Start pos set to: "<<currentPos.x()<<" "<<currentPos.y()<<" "<<currentPos.z()<<std::endl;
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
    startQuat = quat;
    startEuler = startQuat.toEulerAngles();
    emit startQuatChanged();
    emit startEulerChanged();
}

void Simulation::setEndQuat(const QQuaternion& quat) {
    endQuat = quat;
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
    auto div = to-from;
    if(std::abs(div)>180){
        div = 180-div;
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

QQuaternion lerp(double at, const QQuaternion& from, const QQuaternion& to){
    return (from*(1-at) + to*at).normalized();
}

void Simulation::computeCurrentData(){
    double at = time/animationTime;
    std::cout<<"at: "<<at<<std::endl;
    double mat = 1-at;

    currentEuler = interpolateEuler(at, startEuler, endEuler);
    currentQuat = lerp(at, startQuat, endQuat);
    currentPos = startPos*mat + endPos * at;
    //std::cout<<"Start pos set to: "<<startPos.x()<<" "<<startPos.y()<<" "<<startPos.z()<<std::endl;
    //std::cout<<"End pos set to: "<<endPos.x()<<" "<<endPos.y()<<" "<<endPos.z()<<std::endl;
    //std::cout<<"current pos: "<<currentPos.x()<<" "<<currentPos.y()<<" "<<currentPos.z()<<std::endl;

    emit currentStateChanged();
}
