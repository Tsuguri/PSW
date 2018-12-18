#include "ParticleModel.hpp"

#include <random>

const float ParticleModel::Extent = 15.0f;
const float ParticleModel::DefaultMass = 0.2f;

ParticleModel::ParticleModel(QObject *parent)
        : QAbstractListModel(parent),
          mass(DefaultMass), bezierBuffer(nullptr) {

    auto de = Extent / (size - 1);

    for (std::size_t x = 0; x < size; ++x)
    {
        for (std::size_t y = 0; y < size; ++y)
        {
            for (std::size_t z = 0; z < size; ++z)
            {
                auto &particle = particles[x][y][z];
                particle.mass = DefaultMass;
                particle.force = QVector3D(0, 0, 0);
                particle.velocity = QVector3D(0, 0, 0);
                particle.position = QVector3D(x * de, y * de, z * de) - QVector3D(Extent / 2, Extent / 2, Extent / 2);
                particle.previousPosition = particle.position;
            }
        }
    }
}

void ParticleModel::disturb() {
    auto de = 3 * Extent / (size - 1);

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(-de, de);

    for (std::size_t x = 0; x < size; ++x) {
        for (std::size_t y = 0; y < size; ++y) {
            for (std::size_t z = 0; z < size; ++z) {
                auto &particle = particles[x][y][z];
                particle.velocity = QVector3D(static_cast<float>(dist(e2)), static_cast<float>(dist(e2)), static_cast<float>(dist(e2)));
            }
        }
    }

}

QHash<int, QByteArray> ParticleModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[VelocityRole] = "velocity";
    roles[ForceRole] = "force";
    roles[MassRole] = "mass";
    return roles;
}

QVariant ParticleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto row = index.row();

    auto x = row / (size * size);
    row %= (size * size);
    auto y = row / size;
    row %= size;
    auto z = row;

    auto &particle = particles[x][y][z];

    switch (role) {
        case PositionRole:
            return QVariant(particle.position);
        case VelocityRole:
            return QVariant(particle.velocity);
        case ForceRole:
            return QVariant(particle.force);
        case MassRole:
            return QVariant(particle.mass);
        default:
            return QVariant();
    }
}

struct controlPoint {
    float x, y, z, w;
};

struct bezierBuffer {
    controlPoint bezierPositions[64];
};

void ParticleModel::fillBezierBuffer() {
    if (bezierBuffer == nullptr)
        return;

    struct bezierBuffer buffer;

    for (std::size_t x = 0; x < size; ++x) {
        for (std::size_t y = 0; y < size; ++y) {
            for (std::size_t z = 0; z < size; ++z) {
                buffer.bezierPositions[(x * size * size + y * size + z)].x =  particles[x][y][z].position.x();
                buffer.bezierPositions[(x * size * size + y * size + z)].y =  particles[x][y][z].position.y();
                buffer.bezierPositions[(x * size * size + y * size + z)].z =  particles[x][y][z].position.z();
                buffer.bezierPositions[(x * size * size + y * size + z)].w = 1.0f;
            }
        }
    }

    bezierBuffer->setData(QByteArray(reinterpret_cast<char*>(&buffer), sizeof(struct bezierBuffer)));
}
