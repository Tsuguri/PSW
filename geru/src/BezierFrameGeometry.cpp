#include "BezierFrameGeometry.hpp"


BezierFrameGeometry::BezierFrameGeometry(Qt3DCore::QNode *parent)
        : Qt3DRender::QGeometry(parent),
          particleModel(nullptr)
{
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
}

void BezierFrameGeometry::setParticleModel(ParticleModel *value) {
    if (particleModel != value) {
        if (particleModel != nullptr)
            QObject::disconnect(particleModel, &ParticleModel::dataChanged, this, &BezierFrameGeometry::modelChanged);
        particleModel = value;
        if (particleModel != nullptr)
            QObject::connect(particleModel, &ParticleModel::dataChanged, this, &BezierFrameGeometry::modelChanged);
        emit particleModelChanged();
    }
}

void BezierFrameGeometry::modelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                       const QVector<int> &roles) {
    regenerate();
}

void BezierFrameGeometry::regenerate() {

    if (particleModel == nullptr) {
        indexAttr->setCount(0);
        positionAttr->setCount(0);
        normalAttr->setCount(0);
        return;
    }

    const auto verticesCount = ParticleModel::size * ParticleModel::size * ParticleModel::size;

    vertex vb[verticesCount];

    auto indexFunc = [](std::size_t x, std::size_t y, std::size_t z) {
        return static_cast<unsigned int>((x * ParticleModel::size + y) * ParticleModel::size + z);
    };

    std::vector<unsigned int> ib;
    ib.reserve(360);

    for (std::size_t x = 0; x < ParticleModel::size; ++x) {
        for (std::size_t y = 0; y < ParticleModel::size; ++y) {
            for (std::size_t z = 0; z < ParticleModel::size; ++z) {
                auto &particle = particleModel->getParticle(x, y, z);

				vb[indexFunc(x, y, z)] = { { particle.position.x(), particle.position.y(), particle.position.z() }, { 0.0f, 0.0f, 0.0f }};
               
                if (x < ParticleModel::size - 1) {
                    ib.push_back(indexFunc(x, y, z));
                    ib.push_back(indexFunc(x + 1, y, z));
                }

                if (y < ParticleModel::size - 1) {
                    ib.push_back(indexFunc(x, y, z));
                    ib.push_back(indexFunc(x, y + 1, z));
                }

                if (z < ParticleModel::size - 1) {
                    ib.push_back(indexFunc(x, y, z));
                    ib.push_back(indexFunc(x, y, z + 1));
                }

//                if (x < ParticleModel::size - 1 && y < ParticleModel::size - 1) {
//                    ib.push_back(indexFunc(x, y, z));
//                    ib.push_back(indexFunc(x + 1, y + 1, z));
//                    ib.push_back(indexFunc(x + 1, y, z));
//                    ib.push_back(indexFunc(x, y + 1, z));
//                }
//
//                if (x < ParticleModel::size - 1 && z < ParticleModel::size - 1) {
//                    ib.push_back(indexFunc(x, y, z));
//                    ib.push_back(indexFunc(x + 1, y, z + 1));
//                    ib.push_back(indexFunc(x + 1, y, z));
//                    ib.push_back(indexFunc(x, y, z + 1));
//                }
//
//                if (y < ParticleModel::size - 1 && z < ParticleModel::size - 1) {
//                    ib.push_back(indexFunc(x, y, z));
//                    ib.push_back(indexFunc(x, y + 1, z + 1));
//                    ib.push_back(indexFunc(x, y + 1, z));
//                    ib.push_back(indexFunc(x, y, z + 1));
//                }
            }
        }
    }

    ibo->setData(QByteArray(reinterpret_cast<const char *>(ib.data()), static_cast<int>(ib.size() * sizeof(unsigned int))));
    vbo->setData(QByteArray(reinterpret_cast<const char *>(vb), static_cast<int>(sizeof(vb))));

    indexAttr->setCount(static_cast<unsigned>(ib.size()));
    positionAttr->setCount(static_cast<unsigned>(verticesCount));
    normalAttr->setCount(static_cast<unsigned>(verticesCount));
}

