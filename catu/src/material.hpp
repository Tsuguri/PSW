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
   float z;
   float y;
};

struct vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoord;

    static const size_t positionOffset = 0;
    static const size_t normalOffset = sizeof(position);
    static const size_t texOffset = normalOffset + sizeof(normal);
    static const size_t size = sizeof(texCoord) + texOffset;
};


class Material : public Qt3DCore::QEntity {
    Q_OBJECT


    public:
        explicit Material(Qt3DCore::QEntity* parent = nullptr);


    private:

        void resize(float xSize, float ySize, float height, unsigned int xResolution, unsigned int yResolution);

        std::vector<vertex> buffer;
        vertex* minusXbottom;
        vertex* minusYbottom;
        vertex* plusXbottom;
        vertex* plusYbottom;
        vertex* minusXup;
        vertex* minusYup;
        vertex* plusXup;
        vertex* plusYup;

        std::vector<unsigned int> indexBuffer;
        Qt3DCore::QTransform* transform;

        unsigned int xRes, yRes;
        vec3 gridSize;

        Qt3DRender::QBuffer* vbo;
        Qt3DRender::QBuffer* ibo;

        Qt3DRender::QAttribute* positionAttr;
        Qt3DRender::QAttribute* normalAttr;
        Qt3DRender::QAttribute* uvAttr;
        Qt3DRender::QAttribute* indexAttr;
};
#endif