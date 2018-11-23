#pragma once

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include "../../catu/include/math/vec.hpp"
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

struct vert {
    vec3 position;
};
class Trace: public Qt3DCore::QEntity {
    Q_OBJECT

    Q_PROPERTY(int traceLen READ getLen WRITE setLen NOTIFY lenChanged);


    public:
        explicit Trace(Qt3DCore::QEntity* parent = nullptr);

    Q_INVOKABLE void reset();
        void setLen(int len);

        int getLen() const;

    public slots:
        void newPoint(QVector3D vec);

signals:
    void lenChanged();
    private:
        void generateBuffer();
        Qt3DCore::QTransform* _transform;

        Qt3DRender::QBuffer* vbo;

        Qt3DRender::QAttribute* positionAttr;

        int traceLen;

        std::vector<vert> positions;
};

