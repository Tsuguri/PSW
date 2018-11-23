#include "trace.hpp"
#include <iostream>

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QDiffuseMapMaterial>

#include <Qt3DExtras/QPerVertexColorMaterial>

Trace::Trace(Qt3DCore::QEntity* parent) : Qt3DCore::QEntity(parent) {
    auto geometry_renderer = new Qt3DRender::QGeometryRenderer(this);
    geometry_renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::LineStrip);
    auto geometry = new Qt3DRender::QGeometry(geometry_renderer);
    vbo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
    //ibo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, geometry);

    positionAttr = new Qt3DRender::QAttribute(geometry);
    positionAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttr->setBuffer(vbo);
    positionAttr->setByteOffset(0);
    positionAttr->setByteStride(sizeof(vert));
    positionAttr->setVertexSize(3);
    positionAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::Float);
    positionAttr->setCount(0);
    positionAttr->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    /*indexAttr = new Qt3DRender::QAttribute(geometry);
    indexAttr->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttr->setBuffer(ibo);
    indexAttr->setByteOffset(0);
    indexAttr->setByteStride(sizeof(unsigned int));
    indexAttr->setVertexSize(1);
    indexAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::UnsignedInt);
    indexAttr->setCount(0);
*/
    geometry->addAttribute(positionAttr);
    //geometry->addAttribute(indexAttr);
    geometry_renderer->setGeometry(geometry);

    auto material = new Qt3DExtras::QPerVertexColorMaterial(this);
    this->addComponent(geometry_renderer);
    this->addComponent(material);
}

void Trace::setLen(int len) {
    traceLen = len;
    std::cout<<traceLen<<std::endl;
    emit lenChanged();
}

int Trace::getLen() const {
    return traceLen;
}


void Trace::newPoint(QVector3D vec) {

    positions.insert(positions.begin(),vert{vec3{vec.x(), vec.z(), vec.y()}});
    while(positions.size()>traceLen) {
        positions.pop_back();
    }

    generateBuffer();
}

void Trace::generateBuffer() {

    vbo->setData(QByteArray(reinterpret_cast<const char *>(positions.data()), static_cast<int>(positions.size() * sizeof(vert))));

    positionAttr->setCount(static_cast<unsigned int>(positions.size()));

}

void Trace::reset() {
    positions.clear();
    generateBuffer();
}
