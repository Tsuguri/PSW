#ifndef PSW_BEZIER_FRAME_GEOMETRY_HPP
#define PSW_BEZIER_FRAME_GEOMETRY_HPP

#include <vector>

#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>

#include "ParticleModel.hpp"

class BezierFrameGeometry : public Qt3DRender::QGeometry {
    Q_OBJECT

    Q_PROPERTY(ParticleModel* particleModel READ getParticleModel WRITE setParticleModel NOTIFY particleModelChanged);
public:
    explicit BezierFrameGeometry(Qt3DCore::QNode *parent = nullptr);

    ParticleModel* getParticleModel() const { return particleModel; }

    void setParticleModel(ParticleModel* value);

signals:
    void particleModelChanged();
private slots:
    void modelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
private:
    ParticleModel* particleModel;

    struct vertex {
        float position[3];
        float normal[3];
    };

    Qt3DRender::QBuffer *vbo, *ibo;
    Qt3DRender::QAttribute *positionAttr, *indexAttr, *normalAttr;

    void regenerate();
};

#endif //PSW_BEZIER_FRAME_GEOMETRY_HPP
