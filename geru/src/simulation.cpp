#include "simulation.hpp"

Point::Point(QObject* parent) : QObject(parent), pos(), vel(), acc() {
}

QVector3D Point::getPos() const {
    return pos;
}

QVector3D Point::getVel() const {
    return vel;
}

QVector3D Point::getAcc() const {
    return acc;
}

void Point::setPos(QVector3D val) {
    pos = val;
    emit posChanged();
}

void Point::setVel(QVector3D val) {
    vel = val;
    emit velChanged();
}

void Point::setAcc(QVector3D val) {
    acc = val;
    emit accChanged();
}

Spring::Spring(QObject* parent) : QObject(parent) {
}

void Spring::setStart(Point* val) {
    startPoint = val;
    emit startChanged();
}

void Spring::setEnd(Point* val) {
    endPoint = val;
    emit endChanged();
}

void Spring::setLength(float val) {
    desiredLength = val;
}

Point* Spring::getStart() const {
    return startPoint;
}

Point* Spring::getEnd() const {
    return endPoint;
}

Simulation::Simulation(QObject* parent) : QObject(parent) {
    static constexpr float ext   = 15.0f;
    static constexpr int size    = 4;
    static constexpr float d     = ext / (size - 1);
    static constexpr QVector3D c = QVector3D(ext / 2, ext / 2, ext / 2);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++) {
                auto pt = new Point(this);
                pt->setVel(QVector3D(0, 0, 0));
                pt->setAcc(QVector3D(0, 0, 0));
                pt->setPos(QVector3D(i * d, j * d, k * d) - c);
                pts.push_back(pt);
            }
    emit pointsChanged();

    auto spr = new Spring(this);
    spr->setStart(pts[0]);
    spr->setEnd(pts[1]);
    spr->setLength(d);
    sprs.push_back(spr);
    emit springsChanged();
}

QQmlListProperty<Point> Simulation::getPoints() {
    return QQmlListProperty<Point>(this, pts);
}

QQmlListProperty<Spring> Simulation::getSprings() {
    return QQmlListProperty<Spring>(this, sprs);
}

const QList<Spring*>& Simulation::borrowSprings() const {
    return sprs;
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
