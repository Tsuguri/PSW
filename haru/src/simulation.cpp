#include "simulation.hpp"

Simulation::Simulation(QObject* parent) : QObject(parent), running(false), time(0.0), timer(new QTimer(this)) {
    QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
}

Simulation::~Simulation(){}


bool Simulation::getRunning() const {
    return running;
}

bool Simulation::getPaused() const {
    return paused;
}

void Simulation::togglePause() {
    if(!running){
        return;
    }
    if(!paused){
        timer->stop();
    
    }else{
        timer->start();
    }
    paused = !paused;
    emit pausedChanged();
}

void Simulation::runSimulation(double dt, double mass) {
    timer->setInterval(static_cast<int>(dt*1000));
    time = 0;
    this->dt = dt;
    timer->start();
    running = true;
    paused = false;
    emit runningChanged();
    emit pausedChanged();
}

void Simulation::reset() {
    timer->stop();
    running = false;
    paused = false;
    time = 0;
    emit positionChanged();
    emit runningChanged();
    emit pausedChanged();
}


void Simulation::tick() {

    time += dt;
    emit positionChanged();
}


double Simulation::getTime() const {
    return 0.0;
}

double Simulation::getPosition() const {
    return std::sin(time);
}

double Simulation::getVelocity() const {
    return 0.0;
}
double Simulation::getAcceleration() const {
    return 0.0;
}
