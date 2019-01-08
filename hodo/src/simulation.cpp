#include "simulation.hpp"
#include <cmath>
#include <iostream>

Simulation::Simulation(QObject* parent)
    : QObject(parent),
      running(false),
      time(0.0),
      timer(new QTimer(this)),
      dt(0.01),
      w(0),
      r(1),
      l(2),
      elapsed(new QElapsedTimer),
      elapsedNotUsed(0) {
    QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
}

Simulation::~Simulation() {
}

bool Simulation::getRunning() const {
    return running;
}

bool Simulation::getPaused() const {
    return paused;
}

void Simulation::togglePause() {
    if (!running) {
        return;
    }
    if (!paused) {
        timer->stop();

    } else {
        elapsed->restart();
        timer->start();
        elapsedNotUsed = 0;
    }
    paused = !paused;
    emit pausedChanged();
}

void Simulation::runSimulation(double dt) {
    timer->setInterval(static_cast<int>(dt * 1000));
    time     = 0;
    this->dt = dt;

    current  = {0, 0, 0};
    previous = {0, 0, 0};
    timer->start();
    elapsed->restart();
    running = true;
    paused  = false;

    emit runningChanged();
    emit pausedChanged();
}

void Simulation::StateChanged() {
    emit positionChanged();
    emit velocityChanged();
    emit accelerationChanged();
    emit angleChanged();
}

void Simulation::reset() {
    timer->stop();
    running = false;
    paused  = false;
    time    = 0;
    StateChanged();

    emit runningChanged();
    emit pausedChanged();
}

void Simulation::tick() {

    //time += dt;

    auto nextState = State{std::sin(time), std::cos(time), -std::sin(time)};
    currentAngle   = std::fmod(time, M_PI * 2);

    auto elapsedTime = elapsed->elapsed() + elapsedNotUsed;
    elapsed->restart();
    auto interval = timer->interval();

    while (elapsedTime > interval) {
        elapsedTime -= interval;
        time += interval * 0.001;

        previous = current;
        current  = nextState;
    }
    elapsedNotUsed = elapsedTime;

    StateChanged();
    emit stepMade();
    emit timeChanged();
}

double Simulation::getTime() const {
    return time;
}

double Simulation::getPosition() const {
    return current.position;
}

double Simulation::getVelocity() const {
    return current.velocity;
}

double Simulation::getAcceleration() const {
    return current.acceleration;
}

void Simulation::setW(double val) {
    w = val;
    emit wChanged();
}

void Simulation::setL(double val) {
    l = val;
    emit lChanged();
}

void Simulation::setR(double val) {
    r = val;
    std::cout << "setr: " << val;
    emit rChanged();
}

double Simulation::getAngle() const {
    return currentAngle;
}

double Simulation::getW() const {
    return w;
}

double Simulation::getR() const {
    return r;
}

double Simulation::getL() const {
    return l;
}