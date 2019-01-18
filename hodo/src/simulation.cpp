#include "simulation.hpp"
#include <cmath>
#include <iostream>
#include <random>

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
      elapsedNotUsed(0),
      gen(rd())
{
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
    timer->start();
    elapsed->restart();
    running = true;
    paused  = false;
    currentAngle=0;
    emit stateChanged();

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


     std::normal_distribution<double> errorDistribution{
         0.0, err> 0.0 ? err: 1.0};

    auto elapsedTime = elapsed->elapsed() + elapsedNotUsed;
    elapsed->restart();
    auto interval = timer->interval();

    while (elapsedTime > interval) {
        elapsedTime -= interval;
        auto dt = interval * 0.001;
        time += dt;

        currentAngle += dt*w;
        currentAngle = std::fmod(currentAngle, 2*M_PI);
        auto error = err > 0.0 ? errorDistribution(gen) : 0.0;

        auto nextPos = massPosition(r,l + error, currentAngle);
        auto nextVel = (nextPos - current.position)/ dt;
        auto nextAcc = (nextPos - current.position * 2 + previous.position)/ dt / dt;
        //auto nextAcc = ((nextPos - previous.position)/dt - (previous.position - prevPrev.position)/dt)/dt;
        //std::cout<<"n: "<<nextPos<<" prev: "<<previous.position<<" pp: "<<prevPrev.position<<std::endl;
        auto nextState = State{nextPos, nextVel, nextAcc};

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
    emit rChanged();
}

void Simulation::setErr(double val) {
    err = val;
    emit errChanged();
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

double Simulation::getErr() const {
    return err;
}
