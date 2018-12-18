#include "CuboidWireframeGeometry.hpp"

const float CuboidWireframeGeometry::DefaultExtent = 2.0f;

CuboidWireframeGeometry::CuboidWireframeGeometry(Qt3DCore::QNode *parent)
        : Qt3DRender::QGeometry(parent),
          xExtent(DefaultExtent),
          yExtent(DefaultExtent),
          zExtent(DefaultExtent)
{
    vbo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this);
    ibo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, this);

    vbo->setUsage(Qt3DRender::QBuffer::StaticDraw);
    ibo->setUsage(Qt3DRender::QBuffer::StaticDraw);

    positionAttr = new Qt3DRender::QAttribute(this);
    positionAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttr->setBuffer(vbo);
    positionAttr->setByteOffset(0);
    positionAttr->setByteStride(sizeof(vertex));
    positionAttr->setVertexSize(3);
    positionAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::Float);
    positionAttr->setCount(0);
    positionAttr->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    normalAttr = new Qt3DRender::QAttribute(this);
    normalAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    normalAttr->setBuffer(vbo);
    normalAttr->setByteOffset(sizeof(vertex::position));
    normalAttr->setByteStride(sizeof(vertex));
    normalAttr->setVertexSize(3);
    normalAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::Float);
    normalAttr->setCount(0);
    normalAttr->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());

    indexAttr = new Qt3DRender::QAttribute(this);
    indexAttr->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttr->setBuffer(ibo);
    indexAttr->setByteOffset(0);
    indexAttr->setByteStride(sizeof(unsigned int));
    indexAttr->setVertexSize(1);
    indexAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::UnsignedInt);
    indexAttr->setCount(0);

    this->addAttribute(positionAttr);
    this->addAttribute(normalAttr);
    this->addAttribute(indexAttr);

    regenerate();
}

void CuboidWireframeGeometry::regenerate() {

    auto dx = xExtent / 2;
    auto dy = yExtent / 2;
    auto dz = zExtent / 2;

    vertex vb[8] = {
        {{-dx, -dy, -dz}, {0, 0, 0}},
        {{+dx, -dy, -dz}, {0, 0, 0}},
        {{+dx, -dy, +dz}, {0, 0, 0}},
        {{-dx, -dy, +dz}, {0, 0, 0}},
        {{-dx, +dy, -dz}, {0, 0, 0}},
        {{+dx, +dy, -dz}, {0, 0, 0}},
        {{+dx, +dy, +dz}, {0, 0, 0}},
        {{-dx, +dy, +dz}, {0, 0, 0}},
    };

    unsigned int ib[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
    };

    ibo->setData(QByteArray(reinterpret_cast<const char *>(ib), static_cast<int>(sizeof(ib))));
    vbo->setData(QByteArray(reinterpret_cast<const char *>(vb), static_cast<int>(sizeof(vb))));

    indexAttr->setCount(24);
    positionAttr->setCount(8);
    normalAttr->setCount(8);
}
