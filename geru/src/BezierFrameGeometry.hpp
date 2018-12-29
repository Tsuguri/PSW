#ifndef PSW_BEZIER_FRAME_GEOMETRY_HPP
#define PSW_BEZIER_FRAME_GEOMETRY_HPP

#include <vector>

#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>

#include "simulation.hpp"

class BezierFrameGeometry : public Qt3DRender::QGeometry {
    Q_OBJECT

    Q_PROPERTY(
        Simulation* simulation READ getSimulation WRITE setSimulation NOTIFY simulationChanged);

  public:
    explicit BezierFrameGeometry(Qt3DCore::QNode* parent = nullptr);

    Simulation* getSimulation() const {
        return simulation;
    }

    void setSimulation(Simulation* value);

  signals:
    void simulationChanged();

  private:
    Simulation* simulation;

    struct vertex {
        float position[3];
        // float normal[3];
    };

    Qt3DRender::QBuffer *vbo, *ibo;
    Qt3DRender::QAttribute *positionAttr, *indexAttr;  //, *normalAttr;

    void regenerate();
};

#endif  //PSW_BEZIER_FRAME_GEOMETRY_HPP
