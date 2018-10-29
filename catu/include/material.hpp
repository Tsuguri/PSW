#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

#include <vector>
#include <tools/mill.hpp>
#include <math/vec.hpp>

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


        Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged);
    public:
        explicit Material(Qt3DCore::QEntity* parent = nullptr);

        bool isModified() const;

        void mill(Mill* tool, vec3 from, vec3 to, bool updateBuffer = true);
        void updateNormals();
        void updateNormals(float xFrom, float xTo, float yFrom, float yTo);
        void sendVertices();

        Q_INVOKABLE void resetHeights();

    private:

        void millPlace(vec3 pos, float radius, const std::function<float(float, float)>& lamb);
        vec3 trans(vec3);
        void resize(float xSize, float ySize, float height, unsigned int xResolution, unsigned int yResolution);
        void updateNormal(int x, int y);
        void setHeight(int x, int y, float newHeight);

        bool modified;
        std::vector<vertex> buffer;
        QByteArray tempArray;
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
signals:
        void modifiedChanged();
};
#endif
