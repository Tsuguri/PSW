#include "simulation.hpp"
#include <iostream>
#include <cmath>

Simulation::Simulation(QObject *parent)
    : QObject(parent), running(false), time(0.0), timer(new QTimer(this)),
      dt(0.01), flex(0), damping(0), mass(1), elapsed(new QElapsedTimer),
      elapsedNotUsed(0) {
  QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
}

Simulation::~Simulation() {}

bool Simulation::getRunning() const { return running; }

bool Simulation::getPaused() const { return paused; }

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

void Simulation::runSimulation(double dt, double mass, double p0, double v0,
                               double damping, double flx, MathFormula* externalForce, MathFormula* originMovement) {
  timer->setInterval(static_cast<int>(dt * 1000));
  time = 0;
  this->dt = dt;
  this->flex = flx;
  this->damping = damping;
  this->mass = mass;
  this->originMovement = originMovement;
  this->externalForce = externalForce;

  current = State{p0, v0, 0};
  previous = State{p0 - dt * v0, v0, -dt};

  timer->start();
  elapsed->restart();
  running = true;
  paused = false;

  emit massChanged();
  emit runningChanged();
  emit pausedChanged();
}

void Simulation::StateChanged() {
  emit positionChanged();
  emit velocityChanged();
  emit accelerationChanged();
  emit flexForceChanged();
  emit dampeningForceChanged();
  emit externalForceChanged();
  emit originChanged();
}

void Simulation::reset() {
  timer->stop();
  running = false;
  paused = false;
  time = 0;
  StateChanged();

  emit runningChanged();
  emit pausedChanged();
}

void Simulation::tick() {

  //time += dt;

  auto nextState = State{std::sin(time), std::cos(time), time};

  auto elapsedTime = elapsed->elapsed() + elapsedNotUsed;
  elapsed->restart();
  auto interval = timer->interval();

  while (elapsedTime > interval) {
    elapsedTime -= interval;
    time += interval * 0.001;
    // TUTAJ JEST CAŁA WAŻNA MATMA
    // =======================================================

    auto dt2 = dt * dt;
    // acc = (x(t+1) - 2*x(t)+ x(t-1))/dt^2
    // ==>
    // x(t+1) = dt^2 * acc + 2*x(t) - x(t-1)
    nextState.position =
        dt2 * getAcceleration() + 2 * current.position - previous.position;

    nextState.velocity = (nextState.position - current.position) / dt;

    // KONIEC MATMY
    // =======================================================================
    previous = current;
    current = nextState;
  }
  elapsedNotUsed = elapsedTime;

  StateChanged();
  emit stepMade();
  emit timeChanged();
}

double Simulation::getTime() const { return time; }

double Simulation::getPosition() const { return current.position; }

double Simulation::getVelocity() const { return current.velocity; }
double Simulation::getAcceleration() const { return getForce() / mass; }

double Simulation::getFlexForce() const { return flex * (-getOriginPosition() -getPosition()); }

double Simulation::getDampeningForce() const {
  return -damping * getVelocity();
}

double Simulation::getExternalForce() const { return externalForce ? externalForce->evaluate(current.time): 0.0; }

double Simulation::getForce() const {
  return getFlexForce() + getDampeningForce() + getExternalForce();
}

double Simulation::getMass() const { return mass; }
double Simulation::getOriginPosition() const {
    return originMovement ? originMovement->evaluate(current.time) : 0.0;
}
