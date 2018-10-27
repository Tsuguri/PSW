#include <models/path_render.hpp>

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QDiffuseMapMaterial>

#include <Qt3DExtras/QPerVertexColorMaterial>


PathRender::PathRender(Qt3DCore::QEntity* parent) : Qt3DCore::QEntity(parent), _path(), _manager(nullptr) {

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

    generateVertices();
}

PathManager* PathRender::getPaths() const {
    return _manager;
}

void PathRender::setPaths(PathManager* manager) {
    if(_manager != nullptr) {
        // disconnect onPathsChanges, czy cośtam
    disconnect(_manager, &PathManager::pathsChanged, this, &PathRender::onPathsChanged);
    }
    _manager = manager;

    connect(_manager, &PathManager::pathsChanged, this, &PathRender::onPathsChanged);

    // connect pathsChanges czy cośtam

}

void PathRender::onPathsChanged() {
    std::cout<<"Paths changed"<<std::endl;
    generateVertices();
}

void PathRender::generateVertices() {
    if(_manager == nullptr) {
        _path.resize(0);
    } else {
        // generate paths
        unsigned int steps = 0;
        for(unsigned int i =0; i<_manager->pathsCount(); i++) {
            const auto path = _manager->path(i);
            steps += path->points();
        }

        _path.clear();
        _path.reserve(steps);
        for(unsigned int  i= 0; i<_manager->pathsCount(); i++) {
            _manager->path(i)->enumeratePositions([this](auto pos) {
                _path.push_back(vert{pos});
                std::cout<<"x: "<<pos.x<<" y: "<<pos.y<<" z: "<<pos.z<<std::endl;
            });
        }

    }
    std::cout<<"new number of vertices: "<<_path.size()<<std::endl;

    vbo->setData(QByteArray(reinterpret_cast<const char *>(_path.data()), static_cast<int>(_path.size() * sizeof(vert))));

    positionAttr->setCount(static_cast<unsigned int>(_path.size()));


}
