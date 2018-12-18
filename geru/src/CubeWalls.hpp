#ifndef PSW_CUBEWALLS_HPP
#define PSW_CUBEWALLS_HPP

#include <QObject>

#include "ParticleModel.hpp"

class CubeWalls : public QObject {
    Q_OBJECT

    Q_PROPERTY(qreal xMin READ getXMin WRITE setXMin NOTIFY changed)
    Q_PROPERTY(qreal xMax READ getXMax WRITE setXMax NOTIFY changed)
    Q_PROPERTY(qreal yMin READ getYMin WRITE setYMin NOTIFY changed)
    Q_PROPERTY(qreal yMax READ getYMax WRITE setYMax NOTIFY changed)
    Q_PROPERTY(qreal zMin READ getZMin WRITE setZMin NOTIFY changed)
    Q_PROPERTY(qreal zMax READ getZMax WRITE setZMax NOTIFY changed)

    Q_PROPERTY(float springiness READ getSpringiness WRITE setSpringiness NOTIFY springinessChanged)
public:
    static const qreal DefaultSize;
    static const float DefaultSpringiness;

    explicit CubeWalls(QObject *parent = nullptr);

    qreal getXMin() const { return xMin; }
    qreal getXMax() const { return xMax; }
    qreal getYMin() const { return yMin; }
    qreal getYMax() const { return yMax; }
    qreal getZMin() const { return zMin; }
    qreal getZMax() const { return zMax; }

    void setXMin(qreal value) {
        xMin = value;
        emit changed();
    }

    void setXMax(qreal value) {
        xMax = value;
        emit changed();
    }

    void setYMin(qreal value) {
        yMin = value;
        emit changed();
    }

    void setYMax(qreal value) {
        yMax = value;
        emit changed();
    }

    void setZMin(qreal value) {
        zMin = value;
        emit changed();
    }

    void setZMax(qreal value) {
        zMax = value;
        emit changed();
    }

    float getSpringiness() const { return springiness; }

    void setSpringiness(float value) {
        springiness = value;
        emit springinessChanged();
    }

    bool processParticle(Particle &particle);

signals:
    void changed();
    void springinessChanged();
private:
    qreal xMin, xMax, yMin, yMax, zMin, zMax;
    float springiness;
};

#endif //PSW_CUBEWALLS_HPP
