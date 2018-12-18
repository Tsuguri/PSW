#include "CubeWalls.hpp"

const qreal CubeWalls::DefaultSize = 30.0;
const float CubeWalls::DefaultSpringiness = 0.2f;

CubeWalls::CubeWalls(QObject* parent)
        : QObject(parent),
          xMin(-DefaultSize), xMax(DefaultSize),
          yMin(-DefaultSize), yMax(DefaultSize),
          zMin(-DefaultSize), zMax(DefaultSize),
          springiness(DefaultSpringiness)
{

}

bool CubeWalls::processParticle(Particle &particle) {

    bool collided = false;

    if (particle.position.x() < xMin) {
        particle.position.setX(xMin + (xMin - particle.position.x()));
		if (particle.velocity.x() < 0)
			particle.velocity.setX(-particle.velocity.x() * springiness);
        collided = true;
    } else if (particle.position.x() > xMax) {
        particle.position.setX(xMax - (particle.position.x() - xMax));
		if (particle.velocity.x() > 0)
			particle.velocity.setX(-particle.velocity.x() * springiness);
        collided = true;
    }

    if (particle.position.y() < yMin) {
        particle.position.setY(yMin + (yMin - particle.position.y()));
		if (particle.velocity.y() < 0)
			particle.velocity.setY(-particle.velocity.y() * springiness);
        collided = true;
    } else if (particle.position.y() > yMax) {
        particle.position.setY(yMax - (particle.position.y() - yMax));
		if (particle.velocity.y() > 0)
			particle.velocity.setY(-particle.velocity.y() * springiness);
        collided = true;
    }

    if (particle.position.z() < zMin) {
        particle.position.setZ(zMin + (zMin - particle.position.z()));
		if (particle.velocity.z() < 0)
	        particle.velocity.setZ(-particle.velocity.z() * springiness);
        collided = true;
    } else if (particle.position.z() > zMax) {
        particle.position.setZ(zMax - (particle.position.z() - zMax));
		if (particle.velocity.z() > 0)
			particle.velocity.setZ(-particle.velocity.z() * springiness);
        collided = true;
    }

    if (collided)
        processParticle(particle);

	return false;
}