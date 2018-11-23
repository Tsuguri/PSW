#include "simulation.hpp"
#include <cmath>


Simulation::Simulation(QObject* parent) : QObject(parent),timer(new QTimer(this)), elapsed(new QElapsedTimer) {

  QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
  timer->setInterval(static_cast<int>(33));
  constantChange = quatFromTo(QVector3D(1,1,1), QVector3D(0,1,0));
  reset();
}

QQuaternion Simulation::getRotation() const {
    return currentRotation*constantChange;
}

float Simulation::getRo() const {
    return ro;
}

float Simulation::getCubeSide() const {
    return  cube;
}

bool Simulation::getRunning() const {
    return running;
}

void Simulation::setRo(float ro) {
    this->ro = ro;
    emit roChanged();
}

void Simulation::setCubeSide( float len) {
    cube = len;
    emit cubeChanged();
}

void Simulation::toggleRun() {
    if(running) {
        timer->stop();
    } else {
        elapsed->restart();
        timer->start();
        elapsedNotUsed = 0;
    }
    running =!running;
    emit runningChanged();
}

void Simulation::reset() {
    if(running) {
        toggleRun();
    }
    time = 0.0f;
    currentRotation = quatFromTo(QVector3D(0,1,0), QVector3D(std::sin(time),1, std::cos(time)));
    emit rotationChanged();
    // reset data to some default
}


void Simulation::tick() {

    auto dt = elapsed->restart();
    time+=dt*0.001f;
    currentRotation = quatFromTo(QVector3D(0,1,0), QVector3D(std::sin(time),1, std::cos(time)));
    emit rotationChanged();

}
