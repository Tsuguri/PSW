#include "simulation.hpp"
#include <QQuaternion>
#include <cmath>
#include <iostream>
#include <random>

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

unsigned int Point::getId() const {
    return id;
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
    // emit accChanged();
}

void Point::addAcc(QVector3D val) {
    acc += val;
}

void Point::setId(unsigned int val) {
    id = val;
}

Spring::Spring(QObject* parent, bool special) : QObject(parent), special(special) {
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

float Spring::getLength() const {
    return desiredLength;
}

bool Spring::isSpecial() const {
    return special;
}

Simulation::Simulation(QObject* parent) : QObject(parent), timer(new QTimer(this)) {
    static constexpr float ext   = 15.0f;
    static constexpr int size    = 4;
    static constexpr float d     = ext / (size - 1);
    static constexpr QVector3D c = QVector3D(ext / 2, ext / 2, ext / 2);
    timer->setInterval(30);
    QObject::connect(timer, &QTimer::timeout, this, &Simulation::tick);

    auto id = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++) {
                auto pt = new Point(this);
                pt->setVel(QVector3D(0, 0, 0));
                pt->setAcc(QVector3D(0, 0, 0));
                pt->setPos(QVector3D(i * d, j * d, k * d) - c);
                pt->setId(id++);
                pts.push_back(pt);
            }

    emit pointsChanged();

    for (int i = 0; i < 8; i++) {
        cubePts[i] = new Point(this);
        cubePts[i]->setId(1200000);
    }

    computeCornerPoints();

    auto f = [](unsigned int x, unsigned int y, unsigned int z) { return (x * 4 + y) * 4 + z; };

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size - 1; k++) {
                // płaskie (nie na krzyż) w każdym kierunku
                auto spr = new Spring(this);
                spr->setStart(pts[f(i, j, k)]);
                spr->setEnd(pts[f(i, j, k + 1)]);
                spr->setLength(d);
                sprs.push_back(spr);

                spr = new Spring(this);
                spr->setStart(pts[f(i, k, j)]);
                spr->setEnd(pts[f(i, k + 1, j)]);
                spr->setLength(d);
                sprs.push_back(spr);

                spr = new Spring(this);
                spr->setStart(pts[f(k, i, j)]);
                spr->setEnd(pts[f(k + 1, i, j)]);
                spr->setLength(d);
                sprs.push_back(spr);
            }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size - 1; j++)
            for (int k = 0; k < size - 1; k++) {
                // j  k
                auto spr = new Spring(this);
                spr->setStart(pts[f(i, j, k)]);
                spr->setEnd(pts[f(i, j + 1, k + 1)]);
                spr->setLength(d * std::sqrt(2));
                sprs.push_back(spr);

                spr = new Spring(this);
                spr->setStart(pts[f(i, j + 1, k)]);
                spr->setEnd(pts[f(i, j, k + 1)]);
                spr->setLength(d * std::sqrt(2));
                sprs.push_back(spr);

                // i j
                spr = new Spring(this);
                spr->setStart(pts[f(k, j, i)]);
                spr->setEnd(pts[f(k + 1, j + 1, i)]);
                spr->setLength(d * std::sqrt(2));
                sprs.push_back(spr);

                spr = new Spring(this);
                spr->setStart(pts[f(k, j + 1, i)]);
                spr->setEnd(pts[f(k + 1, j, i)]);
                spr->setLength(d * std::sqrt(2));
                sprs.push_back(spr);

                // i k
                spr = new Spring(this);
                spr->setStart(pts[f(j, i, k)]);
                spr->setEnd(pts[f(j + 1, i, k + 1)]);
                spr->setLength(d * std::sqrt(2));
                sprs.push_back(spr);

                spr = new Spring(this);
                spr->setStart(pts[f(j + 1, i, k)]);
                spr->setEnd(pts[f(j, i, k + 1)]);
                spr->setLength(d * std::sqrt(2));
                sprs.push_back(spr);
            }

    auto spr = new Spring(this, true);
    spr->setStart(cubePts[0]);
    spr->setEnd(pts[0]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[1]);
    spr->setEnd(pts[3]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[2]);
    spr->setEnd(pts[12]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[3]);
    spr->setEnd(pts[15]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[4]);
    spr->setEnd(pts[48]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[5]);
    spr->setEnd(pts[51]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[6]);
    spr->setEnd(pts[60]);
    spr->setLength(0);
    sprs.push_back(spr);

    spr = new Spring(this, true);
    spr->setStart(cubePts[7]);
    spr->setEnd(pts[63]);
    spr->setLength(0);
    sprs.push_back(spr);
    emit springsChanged();

    timer->start();
}

QQmlListProperty<Point> Simulation::getPoints() {
    return QQmlListProperty<Point>(this, pts);
}

QQmlListProperty<Spring> Simulation::getSprings() {
    return QQmlListProperty<Spring>(this, sprs);
}

float Simulation::getInternalSpring() const {
    return springness;
}

float Simulation::getExternalSpring() const {
    return specialSpringness;
}

float Simulation::getMass() const {
    return mass;
}

float Simulation::getViscose() const {
    return viscosity;
}

float Simulation::getBounce() const {
    return bounce;
}

void Simulation::setBounce(float val) {
    bounce = val;
    emit bounceChanged();
}

const QList<Spring*>& Simulation::borrowSprings() const {
    return sprs;
}

void Simulation::setFramePos(QVector3D val) {
    framePosition = val;
    computeCornerPoints();
    emit framePosChanged();
}

QVector3D Simulation::getFramePos() const {
    return framePosition;
}

void Simulation::setFrameRot(QVector3D val) {
    frameRotation = val;
    computeCornerPoints();
    emit frameRotChanged();
}

QVector3D Simulation::getFrameRot() const {
    return frameRotation;
}

void Simulation::setInternalSpring(float val) {
    springness = val;
    emit internalSpringChanged();
}

void Simulation::setExternalSpring(float val) {
    specialSpringness = val;
    emit externalSpringChanged();
}

void Simulation::setViscose(float val) {
    viscosity = val;
    emit viscoseChanged();
}

void Simulation::setMass(float val) {
    mass = val;
    emit massChanged();
}

void Simulation::computeCornerPoints() {

    int ind = 0;

    for (int i = -1; i <= 1; i += 2)
        for (int j = -1; j <= 1; j += 2)
            for (int k = -1; k <= 1; k += 2) {
                QVector3D pos = {i * 7.5, j * 7.5, k * 7.5};
                auto p2       = QQuaternion::fromEulerAngles(getFrameRot()) * pos;
                cubePts[ind++]->setPos(p2 + getFramePos());
            }
    emit springsChanged();
}

void Simulation::tick() {
    static constexpr float dt = 0.03;
    // std::cout << "tick" << std::endl;

    // reset forces
    for (auto& pt : pts) {
        pt->setAcc(QVector3D(0, 0, 0));

        // viscosity

        auto visc = -pt->getVel() * viscosity / mass;
        pt->addAcc(visc);
    }

    // calculate forces
    for (auto& spr : sprs) {
        auto p1      = spr->getStart()->getPos();
        auto p2      = spr->getEnd()->getPos();
        auto dir     = p2 - p1;
        auto dl      = spr->getLength() - dir.length();
        auto springy = spr->isSpecial() ? specialSpringness : springness;
        dir.normalize();

        auto force = dir * (dl * springy);

        // compute force

        if (spr->getStart()->getId() < 1000) {
            spr->getStart()->addAcc(-force / mass);
            // add force
        }
        if (spr->getEnd()->getId() < 1000) {
            // add force
            spr->getEnd()->addAcc(force / mass);
        }
    }

    // euler
    for (auto& pt : pts) {
        auto acc = pt->getAcc();
        auto dv  = acc * dt;
        pt->setVel(pt->getVel() + dv);
        auto dp = pt->getVel() * dt;
        pt->setPos(pt->getPos() + dp);
    }

    // collision with walls
    collide();

    emit pointsChanged();
    emit springsChanged();
}

void Simulation::randomizePositions() {
    static constexpr float de = 2.0;
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(-de, de);

    for (auto& pt : pts) {
        QVector3D disturbace{static_cast<float>(dist(e2)),
                             static_cast<float>(dist(e2)),
                             static_cast<float>(dist(e2))};
        pt->setPos(pt->getPos() + disturbace);
    }
}

void collideParticle(Point* pt, float bounce) {
    static constexpr float dw = 30.0f;
    bool collision            = false;

    auto pos = pt->getPos();
    auto vel = pt->getVel();

    if (pos.x() > dw) {
        // kolizja z pozytywnym x
        pos.setX(2 * dw - pos.x());
        if (vel.x() > 0) vel.setX(-vel.x() * bounce);
        collision = true;
    }

    if (pos.x() < -dw) {
        // kolizja z negatywnym x
        pos.setX(-2 * dw - pos.x());
        if (vel.x() < 0) vel.setX(-vel.x() * bounce);
        collision = true;
    }

    if (pos.y() > dw) {
        // kolizja z pozytywnym y
        pos.setY(2 * dw - pos.y());
        if (vel.y() > 0) vel.setY(-vel.y() * bounce);
        collision = true;
    }

    if (pos.y() < -dw) {
        // kolizja z negatywnym y
        pos.setY(-2 * dw - pos.y());
        if (vel.y() < 0) vel.setY(-vel.y() * bounce);
        collision = true;
    }

    if (pos.z() > dw) {
        // kolizja z pozytywnym z
        pos.setZ(2 * dw - pos.z());
        if (vel.z() > 0) vel.setZ(-vel.z() * bounce);
        collision = true;
    }

    if (pos.z() < -dw) {
        // kolizja z negatywnym z
        pos.setZ(-2 * dw - pos.z());
        if (vel.z() < 0) vel.setZ(-vel.z() * bounce);
        collision = true;
    }

    pt->setVel(vel);
    pt->setPos(pos);

    //rekurencyjne sprawdzanie
    if (collision) {
        //collideParticle(pt, bounce);
    }
}

void Simulation::collide() {
    for (auto& pt : pts) {
        collideParticle(pt, bounce);
    }
}