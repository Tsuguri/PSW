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

double massPosition(double r, double l, double a) {
    return r * std::cos(a) + std::sqrt(l*l - r*r*std::sin(a)*std::sin(a));

}

void Simulation::runSimulation(double dt) {
    timer->setInterval(static_cast<int>(dt * 1000));
    time     = 0;
    this->dt = dt;

    auto ppPos = massPosition(r,l,-w*2*dt);
    auto pPos = massPosition(r,l,-w*dt);
    auto pos = massPosition(r,l,0);

    current  = {pos, 0, 0};
    previous = {pPos, 0, 0};
    prevPrev = {ppPos,0,0};
    timer->start();
    elapsed->restart();
    running = true;
    paused  = false;

    emit runningChanged();
    emit pausedChanged();
}

void Simulation::StateChanged() {
    emit stateChanged();
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


    auto elapsedTime = elapsed->elapsed() + elapsedNotUsed;
    elapsed->restart();
    auto interval = timer->interval();

    while (elapsedTime > interval) {
        elapsedTime -= interval;
        auto dt = interval * 0.001;
        time += dt;

        currentAngle   = std::fmod(time*3, M_PI * 2);



        auto nextPos = massPosition(r,l,currentAngle);
        auto nextVel = (nextPos - previous.position)/ dt;
        auto nextAcc = (nextPos - previous.position * 2 + prevPrev.position)/ (dt * dt);
        auto nextState = State{nextPos, nextVel, nextAcc};

        prevPrev = previous;
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
