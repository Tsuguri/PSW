#include "simulation.hpp"
#include <cmath>
#include <iostream>

double dt = 0.033;
Simulation::Simulation(QObject *parent)
    : QObject(parent), timer(new QTimer(this)), elapsed(new QElapsedTimer) {

  QObject::connect(timer.get(), &QTimer::timeout, this, &Simulation::tick);
  timer->setInterval(static_cast<int>(33));
  reset();
}

QQuaternion Simulation::getRotation() const {
    // constant change jest po to, żeby wizualizacja była fajnie
    return currentRotation;
}

bool Simulation::getGravity() const { return gravity;}

double Simulation::getRo() const { return ro; }

double Simulation::getCubeSide() const { return cube; }

bool Simulation::getRunning() const { return running; }

double Simulation::getAngleVel() const { return angleVelocity; }

double Simulation::getDiagonalAngle() const { return diagonalAngle; }

void Simulation::setGravity(bool val) {
    gravity = val;
    emit gravityChanged();
}

double Simulation::Mass() const {

    return cube*cube*cube*ro;
}


void Simulation::setRo(double ro) {
  this->ro = ro;
  emit roChanged();
}

void Simulation::setCubeSide(double len) {
  cube = len;
  emit cubeChanged();
}

void Simulation::setDiagonalAngle(double val) {
  diagonalAngle = val;
  if(!running){
    computeStartingPos();
  }
  emit diagonalAngleChanged();
}

void Simulation::setAngleVel(double val) {
  angleVelocity = val;
  emit angleVelChanged();
}

void Simulation::toggleRun() {
  if (running) {
    timer->stop();
  } else {
    elapsed->restart();
    timer->start();
    elapsedNotUsed = 0;

    // tutaj trzeba jakoś zainicjalizować stan początkowy.
    auto initialVel = QVector3D(0,1,0).normalized()*angleVelocity;
    computeStartingPos();
    prev.angleVelocity = initialVel;
    prev.rotation = currentRotation;
    prevPrev.angleVelocity = initialVel;
    prevPrev.rotation = currentRotation;
  }
  running = !running;
  emit runningChanged();
}

void Simulation::reset() {
  if (running) {
    toggleRun();
  }
  time = 0.0f;
  computeStartingPos();
  // reset data to some default
}

QVector3D operator*(const Mat& mat, const QVector3D& vec) {
  const float *d = mat.constData();
  const float *data[] = {d, d + 3, d + 6};
  QVector3D vector{};
  for(int i=0; i<3; i++) {
    vector[i] = data[i][0]*vec[0] + data[i][1]*vec[1]+data[i][2]*vec[2];
  }

  return vector;
}

QVector3D AngleVelMath(const Mat& i, const Mat& iInv, const QVector3D& torque, const QVector3D& prevVel) {

    return iInv * (torque + QVector3D::crossProduct(i*prevVel, prevVel));
}

QQuaternion RotationMath(const QQuaternion& q, const QVector3D& r) {

    //auto r = q*w;
    
    return QQuaternion(0.0, r.x() / 2.0, r.y() / 2.0, r.z() / 2.0)* q.normalized();
}

State RungeKutta(const State& state, const QVector3D& torque, const Mat& i, const Mat& iInv, double dt){
/*
        auto p =state.rotation.inverted()* state.angleVelocity ;
        auto kw1 = dt * AngleVelMath(i, iInv, torque, p);
        auto kw2 = dt * AngleVelMath(i, iInv, torque, p+ kw1 / 2.0);
        auto kw3 = dt * AngleVelMath(i, iInv, torque, p+ kw2 / 2.0);
        auto kw4 = dt * AngleVelMath(i, iInv, torque, p+ kw3);

        auto dw = (kw1+kw2*2+kw3*2+kw4)/6.0;
*/

        State s;
        s.angleVelocity=state.angleVelocity; //state.rotation.inverted() * (p+ dw);
        QQuaternion tmp(1,0,0,0);
        //auto tmp = state.rotation;

        auto p = state.angleVelocity;

        auto kq1 = dt * RotationMath(tmp, p);
        auto kq2 = dt * RotationMath(tmp+kq1/2.0, p);
        auto kq3 = dt * RotationMath(tmp+ kq2/2.0, p);
        auto kq4 = dt * RotationMath(tmp+kq3, p);

        auto dq = (kq1+kq2*2+kq3*2+kq4)/6.0;

        tmp = (tmp+dq).normalized();
        s.rotation = state.rotation*tmp;
        s.rotation.normalize();

        return s;
}

void Simulation::tick() {

  auto dt = elapsed->restart()*0.001;
  time += dt;
  //std::cout<<"dt: "<<dt<<std::endl;
  /*currentRotation = quatFromTo(QVector3D(0, 1, 0),
                               QVector3D(std::sin(time), 1, std::cos(time)));
*/

  auto currentForce = (getRotation().inverted()) * QVector3D(0, 0,-9.81);
  std::cout<<"gravity: "<<currentForce.x()<<" "<<currentForce.y()<<" "<<currentForce.z()<<std::endl;
  currentForce*= gravity? 1.0 : 0.0;


  auto torque = QVector3D::crossProduct(QVector3D(0, cube*std::sqrt(3)/2.0, 0), currentForce);

  auto next = RungeKutta(prev, torque, inertiaTensor, invertedInertiaTensor,  dt);

  prevPrev=prev;
  prev=next;
  currentRotation = next.rotation;
  emit rotationChanged();
}

void Simulation::computeStartingPos() {
    auto ang = diagonalAngle / 180.0 * 3.14;

    currentRotation = quatFromTo(QVector3D(0,1,0), QVector3D(std::sin(ang), std::cos(ang),0).normalized());
    emit rotationChanged();
}

Mat inverse(const Mat &mat) {
  const float *d = mat.constData();
  const float *data[] = {d, d + 3, d + 6};

  auto det = data[0][0] * data[1][1] * data[2][2] +
             data[1][0] * data[2][1] * data[0][2] +
             data[2][0] * data[0][1] * data[1][2] -
             data[0][0] * data[2][1] * data[1][2] -
             data[2][0] * data[1][1] * data[0][2] -
             data[1][0] * data[0][1] * data[2][2];

  float vals[] = {
      (data[1][1] * data[2][2] - data[1][2] * data[2][1]) / det,
      (data[0][2] * data[2][1] - data[0][1] * data[2][2]) / det,
      (data[0][1] * data[1][2] - data[0][2] * data[1][1]) / det,
      (data[1][2] * data[2][0] - data[1][0] * data[2][2]) / det,
      (data[0][0] * data[2][2] - data[0][2] * data[2][0]) / det,
      (data[0][2] * data[1][0] - data[0][0] * data[1][2]) / det,
      (data[1][0] * data[2][1] - data[1][1] * data[2][0]) / det,
      (data[0][1] * data[2][0] - data[0][0] * data[2][1]) / det,
      (data[0][0] * data[1][1] - data[0][1] * data[1][0]) / det};
  return Mat(vals);
}

void Simulation::computeInertia() {

  float sizePowered = cube * cube * cube * cube * cube;
  float diag = 2.0 * ro * sizePowered / 3.0;
  float rest = ro * sizePowered / 4.0;

  float vals[] = {diag, rest, rest, rest, diag, rest, rest, rest, diag};

  auto rot = quatFromTo(QVector3D(1,1,1), QVector3D(0,1,0));
  auto rotMat = rot.toRotationMatrix();
  auto invRot = rot.inverted().toRotationMatrix();

  inertiaTensor = rotMat*Mat(vals)*invRot;

  invertedInertiaTensor = inverse(inertiaTensor);
}
