#ifndef PSW_POINTS_MODEL_HPP
#define PSW_POINTS_MODEL_HPP

#include <array>

#include <QAbstractListModel>
#include <QVector3D>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>

struct Particle {
    QVector3D position;
    QVector3D velocity;
    QVector3D force;
    float mass;
    QVector3D previousPosition;
};

class ParticleModel : public QAbstractListModel {
    Q_OBJECT

    Q_ENUMS(Roles)

    Q_PROPERTY(float extent READ getExtent CONSTANT)

    Q_PROPERTY(float mass READ getMass WRITE setMass NOTIFY massChanged)

    Q_PROPERTY(Qt3DRender::QBuffer* bezierBuffer READ getBezierBuffer WRITE setBezierBuffer NOTIFY bezierBufferChanged)
public:
    static const float Extent;
    static const float DefaultMass;
    static constexpr int size = 4;

    enum Roles {
        PositionRole = Qt::UserRole + 1,
        VelocityRole,
        ForceRole,
        MassRole
    };

    explicit ParticleModel(QObject *parent = nullptr);

    float getExtent() const { return Extent; }

    float getMass() const { return mass; }

    void setMass(float value) {
        mass = value;
        for (std::size_t x = 0; x < size; ++x)
            for (std::size_t y = 0; y < size; ++y)
                for (std::size_t z = 0; z < size; ++z)
                        particles[x][y][z].mass = value;
        emit massChanged();
    }

    Qt3DRender::QBuffer* getBezierBuffer() const { return bezierBuffer; }

    void setBezierBuffer(Qt3DRender::QBuffer* value) {
        if (bezierBuffer != value) {
            bezierBuffer = value;
            emit bezierBufferChanged();
            fillBezierBuffer();
        }
    }

    Q_INVOKABLE void disturb();

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent) const override { return size * size * size; }

    QVariant data(const QModelIndex &index, int role) const override;

    Particle& getParticle(std::size_t x, std::size_t y, std::size_t z) {
        return particles[x][y][z];
    }

    void signalPositionVelocityUpdate() {
        dataChanged(this->index(0, 0), this->index(size * size * size - 1, 0), {PositionRole, VelocityRole});
        fillBezierBuffer();
    }
signals:
    void massChanged();
    void bezierBufferChanged();
private:
    std::array<std::array<std::array<Particle, size>, size>, size> particles;

    float mass;

    Qt3DRender::QBuffer* bezierBuffer;

    void fillBezierBuffer();
};

#endif
