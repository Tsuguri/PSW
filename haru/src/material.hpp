#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

#include <vector>

struct vec2{
    float u;
    float v;
};

struct vec3{
   float x;
   float y;
   float z;
};

struct vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoord;

    static const size_t positionOffset = 0;
    static const size_t normalOffset = sizeof(position);
    static const size_t texOffset = normalOffset + sizeof(normal);
    static const size_t size = sizeof(vertex);
};


class Material : public Qt3DCore::QEntity {
    Q_OBJECT


    public:
        explicit Material(Qt3DCore::QEntity* parent = nullptr);


    private:
        std::vector<vertex> buffer;


        Qt3DRender::QBuffer* vbo;
        Qt3DRender::QBuffer* ibo;

        Qt3DRender::QAttribute* positionAttr;
        Qt3DRender::QAttribute* normalAttr;
        Qt3DRender::QAttribute* uvAttr;
        Qt3DRender::QAttribute* indexAttr;
}
#endif
