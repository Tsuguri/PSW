#include "simulation.hpp"


Simulation::Simulation(QObject* parent) : QObject(parent){

}


void Simulation::setFramePos(QVector3D val) {
    framePosition = val;
    emit framePosChanged();
}

QVector3D Simulation::getFramePos() const {
    return framePosition;
}

void Simulation::setFrameRot(QVector3D val) {
    frameRotation = val;
    emit frameRotChanged();
}

QVector3D Simulation::getFrameRot() const {
    return frameRotation;
}
