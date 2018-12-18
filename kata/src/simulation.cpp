#include "simulation.hpp"
#include <cmath>
#include <iostream>


Simulation::Simulation(QObject* parent) : QObject(parent), a(false), b(false), showOpt(false) {}

bool Simulation::getShowOptions() const {
    return showOpt;
}

int Simulation::getRects() const {
    return rects.size();
}

void Simulation::setR1(float val){
    r1 = val;
    //emit r1Changed();
}

void Simulation::setR2(float val) {
    r2 = val;
    //emit r2Changed();
}

float Simulation::getR1() const {

    return r1;
}


float Simulation::getR2() const {
    return r2;
}
float Simulation::getStartA1() const {
    return startA1;
}

float Simulation::getStartA2() const {
    return startA2;
}

float Simulation::getEndA1() const {
    return endA1;
}

float Simulation::getEndA2() const {
    return endA2;
}

void Simulation::startSimulation() {
    std::cout<<"start simulation<<"<<std::endl;
}

void Simulation::clearRects() {
    std::cout<<"clearing rects"<<std::endl;
}

void Simulation::confirmA() {
    if(selectingA) {
        startA1 = a1a;
        startA2 = a2a;
        emit startChanged();
    } else {
        endA1=a1a;
        endA2 = a2a;
        emit endChanged();
    }

    showOpt = false;
    emit showOptionsChanged();
    // emit change of start/end

}

void Simulation::confirmB() {

    if(selectingA) {
        startA1 = a1b;
        startA2 = a2b;
        emit startChanged();
    } else {
        endA1=a1b;
        endA2 = a2b;
        emit endChanged();
    }

    showOpt = false;
    emit showOptionsChanged();
}

static constexpr float Pi = 3.14;
std::tuple<float, float, float, float> Simulation::InverseKinematics(int x, int y) {
    auto dist = [](int a, int b) {
        return std::sqrt(a*a+b*b);
    };
    float length0 =r1;
    float length1 = r2;
    float length2 = dist(x,y);
    std::cout<<"l1: "<<r1<<" l2: "<<r2<<"l3:" <<length2<<std::endl;
 
    float cosAngle0 = ((length0 * length0) + (length2 * length2) - (length1 * length1)) / (2 * length0 * length2);
    float angle0 = std::acos(cosAngle0)*180/Pi;
    std::cout<<"cos angle0: "<<cosAngle0<<" angle: "<<angle0<<std::endl;
 
    // Inner angle beta
    float cosAngle1 = ((length1 * length1) + (length0 * length0) - (length2 * length2)) / (2 * length1 * length0);
    float angle1 = std::acos(cosAngle1) * 180/Pi;
 
    // Angle from Joint0 and Target
    float atan = std::atan2(y, x) * 180/Pi;
    std::cout<<"atan: "<<atan<<std::endl;
 
    // So they work in Unity reference frame
    float jointAngle0 = (atan - angle0); // Angle A
    float jointAngle1 = (180 - angle1); // Angle B

    float jointAngle0B = (atan + angle0);
    float jointAngle1B = (180 + angle1);

    return std::make_tuple(jointAngle0, jointAngle1, jointAngle0B, jointAngle1B);
}


void Simulation::placeStart(int x, int y) {

    selectingA = true;

    auto angles = InverseKinematics(x,y);
    a1a = std::get<0>(angles);
    a2a = std::get<1>(angles);
    a1b = std::get<2>(angles);
    a2b = std::get<3>(angles);
    std::cout<<"angles: "<<a1a<<" "<<a2a<<" "<<a1b<<" "<<a2b<<std::endl;
    // fill positions
    showOpt = true;
    emit showOptionsChanged();
    emit a1aChanged();
    emit a2aChanged();
    emit a1bChanged();
    emit a2bChanged();

}

void Simulation::placeEnd(int x, int y) {
    selectingA = false;

    auto angles = InverseKinematics(x,y);
    a1a = std::get<0>(angles);
    a2a = std::get<1>(angles);
    a1b = std::get<2>(angles);
    a2b = std::get<3>(angles);
    showOpt = true;
    emit showOptionsChanged();
    emit a1aChanged();
    emit a2aChanged();
    emit a1bChanged();
    emit a2bChanged();
}

float Simulation::getA1A() const {
    return a1a;
}

float Simulation::getA2A() const {
    return a2a;
}

float Simulation::getA1B() const {
    return a1b;
}

float Simulation::getA2B() const {
    return a2b;
}
