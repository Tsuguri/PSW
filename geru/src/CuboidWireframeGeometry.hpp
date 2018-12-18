#ifndef PSW_CUBOID_WIREFRAME_GEOMETRY_HPP
#define PSW_CUBOID_WIREFRAME_GEOMETRY_HPP

#include <vector>

#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>

#include "ParticleModel.hpp"

class CuboidWireframeGeometry : public Qt3DRender::QGeometry {
    Q_OBJECT

    Q_PROPERTY(float xExtent READ getXExtent WRITE setXExtent NOTIFY xExtentChanged)
    Q_PROPERTY(float yExtent READ getYExtent WRITE setYExtent NOTIFY yExtentChanged)
    Q_PROPERTY(float zExtent READ getZExtent WRITE setZExtent NOTIFY zExtentChanged)

public:
    static const float DefaultExtent;

    explicit CuboidWireframeGeometry(Qt3DCore::QNode *parent = nullptr);

    float getXExtent() const { return xExtent; }

    void setXExtent(float value) {
        xExtent = value;
        emit xExtentChanged();
        regenerate();
    }

    float getYExtent() const { return yExtent; }

    void setYExtent(float value) {
        yExtent = value;
        emit yExtentChanged();
        regenerate();
    }

    float getZExtent() const { return zExtent; }

    void setZExtent(float value) {
        zExtent = value;
        emit zExtentChanged();
        regenerate();
    }
signals:
    void xExtentChanged();
    void yExtentChanged();
    void zExtentChanged();
private:
    float xExtent, yExtent, zExtent;
    struct vertex {
        float position[3];
        float normal[3];
    };

    Qt3DRender::QBuffer *vbo, *ibo;
    Qt3DRender::QAttribute *positionAttr, *indexAttr, *normalAttr;

    void regenerate();
};

#endif //PSW_CUBOID_WIREFRAME_GEOMETRY_HPP
