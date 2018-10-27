#pragma once

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

#include <math/vec.hpp>
#include <vector>
#include <models/path_manager.hpp>

struct vert {
    vec3 position;
};


class PathRender : public Qt3DCore::QEntity {
    Q_OBJECT

    Q_PROPERTY(PathManager* paths READ getPaths WRITE setPaths NOTIFY pathsChanged);

    public:
        explicit PathRender(Qt3DCore::QEntity* parent = nullptr);

        PathManager* getPaths() const;
        void setPaths(PathManager* manager);

        public slots:
        void onPathsChanged();

signals:
        void pathsChanged();


    private:
        void generateVertices();
        std::vector<vert> _path;
        PathManager* _manager;

        Qt3DCore::QTransform* _transform;

        Qt3DRender::QBuffer* vbo;

        Qt3DRender::QAttribute* positionAttr;
};
