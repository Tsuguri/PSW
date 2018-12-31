#include "BezierFrameGeometry.hpp"
#include <iostream>

BezierFrameGeometry::BezierFrameGeometry(Qt3DCore::QNode* parent)
    : Qt3DRender::QGeometry(parent), simulation(nullptr) {
    vbo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this);
    ibo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, this);

    vbo->setUsage(Qt3DRender::QBuffer::DynamicDraw);
    ibo->setUsage(Qt3DRender::QBuffer::DynamicDraw);

    positionAttr = new Qt3DRender::QAttribute(this);
    positionAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttr->setBuffer(vbo);
    positionAttr->setByteOffset(0);
    positionAttr->setByteStride(sizeof(vertex));
    positionAttr->setVertexSize(3);
    positionAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::Float);
    positionAttr->setCount(0);
    positionAttr->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    // normalAttr = new Qt3DRender::QAttribute(this);
    // normalAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    // normalAttr->setBuffer(vbo);
    // normalAttr->setByteOffset(sizeof(vertex::position));
    // normalAttr->setByteStride(sizeof(vertex));
    // normalAttr->setVertexSize(3);
    // normalAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::Float);
    // normalAttr->setCount(0);
    // normalAttr->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());

    indexAttr = new Qt3DRender::QAttribute(this);
    indexAttr->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttr->setBuffer(ibo);
    indexAttr->setByteOffset(0);
    indexAttr->setByteStride(sizeof(unsigned int));
    indexAttr->setVertexSize(1);
    indexAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::UnsignedInt);
    indexAttr->setCount(0);

    this->addAttribute(positionAttr);
    // this->addAttribute(normalAttr);
    //this->addAttribute(indexAttr);
    regenerate();
}

void BezierFrameGeometry::setSimulation(Simulation* value) {
    std::cout << "setting simu" << std::endl;
    if (simulation != value) {
        // std::cout << "not the same simu" << std::endl;
        if (simulation != nullptr)
            QObject::disconnect(
                simulation, &Simulation::springsChanged, this, &BezierFrameGeometry::regenerate);
        simulation = value;
        if (simulation != nullptr)
            QObject::connect(
                simulation, &Simulation::springsChanged, this, &BezierFrameGeometry::regenerate);
        emit simulationChanged();
        regenerate();
    }
}

void BezierFrameGeometry::regenerate() {

    if (simulation == nullptr) {
        indexAttr->setCount(0);
        positionAttr->setCount(0);
        // normalAttr->setCount(0);
        // std::cout << "empty simulation" << std::endl;
        return;
    }

    const auto& springs = simulation->borrowSprings();

    const auto verticesCount = springs.size() * 2;
    // std::cout << "there are " << verticesCount << "/2 springs." << std::endl;

    std::vector<vertex> vb;
    vb.reserve(verticesCount);

    auto indexFunc = [](std::size_t x, std::size_t y, std::size_t z) {
        return static_cast<unsigned int>((x * Simulation::size + y) * Simulation::size + z);
    };

    std::vector<unsigned int> ib;
    ib.reserve(verticesCount);
    unsigned int ind = 0;

    for (const auto& elem : springs) {

        auto from = elem->getStart()->getPos();
        auto to   = elem->getEnd()->getPos();
        vb.push_back({from.x(), from.y(), from.z()});
        vb.push_back({to.x(), to.y(), to.z()});

        ib.push_back(ind++);
        ib.push_back(ind++);
    }

    ibo->setData(QByteArray(reinterpret_cast<const char*>(ib.data()),
                            static_cast<int>(ib.size() * sizeof(unsigned int))));
    vbo->setData(QByteArray(reinterpret_cast<const char*>(vb.data()),
                            static_cast<int>(vb.size() * sizeof(vertex))));

    //std::cout << ib.size() << std::endl;
    indexAttr->setCount(static_cast<unsigned>(verticesCount));
    positionAttr->setCount(static_cast<unsigned>(verticesCount));
    // normalAttr->setCount(static_cast<unsigned>(verticesCount));
}
