#include <material.hpp>
#include <iostream>

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QDiffuseMapMaterial>

#include <QTextureImage>

Material::Material(Qt3DCore::QEntity *parent) : Qt3DCore::QEntity(parent), modified(false) {
  auto geometry_renderer = new Qt3DRender::QGeometryRenderer(this);
  auto geometry = new Qt3DRender::QGeometry(geometry_renderer);

  vbo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
  ibo = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, geometry);

  vbo->setUsage(Qt3DRender::QBuffer::DynamicDraw);
  ibo->setUsage(Qt3DRender::QBuffer::DynamicDraw);

  positionAttr = new Qt3DRender::QAttribute(geometry);
  positionAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  positionAttr->setBuffer(vbo);
  positionAttr->setByteOffset(vertex::positionOffset);
  positionAttr->setByteStride(sizeof(vertex));
  positionAttr->setVertexSize(3);
  positionAttr->setVertexBaseType(
      Qt3DRender::QAttribute::VertexBaseType::Float);
  positionAttr->setCount(0);
  positionAttr->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

  normalAttr = new Qt3DRender::QAttribute(geometry);
  normalAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  normalAttr->setBuffer(vbo);
  normalAttr->setByteOffset(vertex::normalOffset);
  normalAttr->setByteStride(sizeof(vertex));
  normalAttr->setVertexSize(3);
  normalAttr->setVertexBaseType(Qt3DRender::QAttribute::VertexBaseType::Float);
  normalAttr->setCount(0);
  normalAttr->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());

  uvAttr = new Qt3DRender::QAttribute(geometry);
  uvAttr->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  uvAttr->setBuffer(vbo);
  uvAttr->setByteOffset(vertex::texOffset);
  uvAttr->setByteStride(sizeof(vertex));
  uvAttr->setVertexSize(2);
  uvAttr->setVertexBaseType(
      Qt3DRender::QAttribute::VertexBaseType::Float);
  uvAttr->setCount(0);
  uvAttr->setName(
      Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName());

  indexAttr = new Qt3DRender::QAttribute(geometry);
  indexAttr->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
  indexAttr->setBuffer(ibo);
  indexAttr->setByteOffset(0);
  indexAttr->setByteStride(sizeof(unsigned int));
  indexAttr->setVertexSize(1);
  indexAttr->setVertexBaseType(
      Qt3DRender::QAttribute::VertexBaseType::UnsignedInt);
  indexAttr->setCount(0);

  geometry->addAttribute(positionAttr);
  geometry->addAttribute(normalAttr);
  geometry->addAttribute(uvAttr);
  geometry->addAttribute(indexAttr);

  geometry_renderer->setGeometry(geometry);

  auto material = new Qt3DExtras::QDiffuseMapMaterial(this);
  auto texture = material->diffuse();
  auto textureImage = new Qt3DRender::QTextureImage();
  textureImage->setSource(QUrl("qrc:/images/texture2.jpg"));
  texture->addTextureImage(textureImage);

  transform = new Qt3DCore::QTransform(this);

  this->addComponent(geometry_renderer);
  this->addComponent(material);
  this->addComponent(transform);

  //regenerate(xExtent, zExtent, height, xCells, zCells, 0);
  resize(150.0f, 150.0f, 50.0f, 300, 300);
}
void Material::resize(float xSize, float ySize, float height, unsigned int xResolution, unsigned int yResolution){
    this->gridSize = vec3{xSize, height, ySize};
    this->xRes = xResolution;
    this->yRes = yResolution;


    auto xVerts = xRes + 1;
    auto yVerts = yRes + 1;


    auto surfaceCount = xVerts * yVerts;
    auto sides = 4*(xVerts + yVerts);
    auto vertices = surfaceCount + sides;

    buffer.resize(vertices);

    auto xd = gridSize.x / xRes;
    auto yd = gridSize.y / yRes;
    
    auto xFrom = -gridSize.x/2.0f;
    auto yFrom = -gridSize.y/2.0f;

    //std::cout<<"xd: "<<xd<<" yd: "<<yd<<"xSize: "<<gridsize.
    // first surfaceCount of vertices -> big quad
    for(unsigned int x = 0; x<xVerts; x++){
        for(unsigned int y = 0; y<yVerts; y++){
            auto& vert = buffer[y*xVerts + x];
            vert.position = vec3{xFrom + x*xd, height, yFrom + y*yd};
            vert.normal = vec3{0.0f, 1.0f, 0.0f};
            vert.texCoord = vec2{x*xd / gridSize.x, y*yd / gridSize.y};
        }
    }

    auto sidesSurface = buffer.data() + surfaceCount;
        auto minusXbottomInd = surfaceCount;
        auto minusXupInd = minusXbottomInd + yVerts;
        auto minusYbottomInd= minusXupInd + yVerts;
        auto minusYupInd = minusYbottomInd + xVerts;
        auto plusXbottomInd = minusYupInd + xVerts;
        auto plusXupInd = plusXbottomInd + yVerts;
        auto plusYbottomInd = plusXupInd + yVerts;
        auto plusYupInd = plusYbottomInd + xVerts;

    minusXbottom = buffer.data() + minusXbottomInd;
    minusXup = buffer.data() + minusXupInd;
    minusYbottom = buffer.data() + minusYbottomInd;
    minusYup = buffer.data() + minusYupInd;

    plusXbottom = buffer.data() + plusXbottomInd;
    plusXup = buffer.data() + plusXupInd;
    plusYbottom = buffer.data() + plusYbottomInd;
    plusYup = buffer.data() + plusYupInd;

    indexBuffer.resize(3*2*xRes * yRes + 6* 2*(xRes + yRes));
    auto data = indexBuffer.data();


    for(unsigned int x = 0; x<xVerts; x++){
        auto xPos = xFrom + x*xd;
        minusYbottom[x].position = vec3{xPos, 0, yFrom};
        minusYbottom[x].normal = vec3{0, 0, -1};
        minusYbottom[x].texCoord = vec2{x*xd/gridSize.x, 0};
        minusYup[x].position = vec3{xPos, height, yFrom};
        minusYup[x].normal = vec3{0, 0, -1};
        minusYup[x].texCoord = vec2{x*xd/gridSize.x, 1};
        plusYbottom[x].position = vec3{xPos, 0, -yFrom};
        plusYbottom[x].normal = vec3{0, 0, 1};
        plusYbottom[x].texCoord = vec2{x*xd/gridSize.x, 0};
        plusYup[x].position = vec3{xPos, height, -yFrom};
        plusYup[x].normal = vec3{0, 0, 1};
        plusYup[x].texCoord = vec2{x*xd/gridSize.x, 1};
    }

    for(unsigned int y = 0; y<yVerts; y++){
        auto yPos = yFrom + y*yd;
        minusXbottom[y].position =vec3{xFrom, 0, yPos};
        minusXbottom[y].normal = vec3{-1, 0, 0};
        minusXbottom[y].texCoord = vec2{y*yd/gridSize.y, 0};
        minusXup[y].position =vec3{xFrom, height, yPos};
        minusXup[y].normal = vec3{-1, 0, 0};
        minusXup[y].texCoord = vec2{y*yd/gridSize.y, 1};
        plusXbottom[y].position =vec3{-xFrom, 0, yPos};
        plusXbottom[y].normal = vec3{1, 0, 0};
        plusXbottom[y].texCoord = vec2{y*yd/gridSize.y, 0};
        plusXup[y].position =vec3{-xFrom, height, yPos};
        plusXup[y].normal = vec3{1, 0, 0};
        plusXup[y].texCoord = vec2{y*yd/gridSize.y, 1};
    }

    for(unsigned int x = 0; x<xRes; x++){
        for(unsigned int y = 0; y<yRes; y++){
            data[(y*xRes + x)*6 + 0] = y *xVerts + x;
            data[(y*xRes + x)*6 + 1] = (y+1)*xVerts +x;
            data[(y*xRes + x)*6 + 2] = y * xVerts + x + 1;
            
            data[(y*xRes + x)*6 + 3] = y *xVerts + x + 1;
            data[(y*xRes + x)*6 + 4] = (y+1)*xVerts +x;
            data[(y*xRes + x)*6 + 5] = (y+1) * xVerts + x + 1;
        }
    }

    auto minusXIndex = indexBuffer.data() + xRes*yRes*6; // yRes
    auto minusYIndex = minusXIndex + yRes * 6; // xRes

    auto plusXIndex = minusYIndex + xRes * 6; // yRes
    auto plusYIndex = plusXIndex + yRes * 6; // xRes

    for(unsigned int x = 0; x<xRes; x++){
        minusYIndex[x*6 + 0] = minusYupInd + x;
        minusYIndex[x*6 + 1] = minusYupInd + x + 1;
        minusYIndex[x*6 + 2] = minusYbottomInd + x;

        minusYIndex[x*6 + 4] = minusYbottomInd + x + 1;
        minusYIndex[x*6 + 3] = minusYupInd + x + 1;
        minusYIndex[x*6 + 5] = minusYbottomInd + x;

        plusYIndex[x*6 + 1] = plusYupInd + x;
        plusYIndex[x*6 + 0] = plusYupInd + x + 1;
        plusYIndex[x*6 + 2] = plusYbottomInd + x;
        plusYIndex[x*6 + 3] = plusYbottomInd + x + 1;
        plusYIndex[x*6 + 4] = plusYupInd + x + 1;
        plusYIndex[x*6 + 5] = plusYbottomInd + x;
    }

    for(unsigned int y=0; y<yRes; y++){
        minusXIndex[y*6 + 0] = minusXupInd + y;
        minusXIndex[y*6 + 2] = minusXupInd + y + 1;
        minusXIndex[y*6 + 1] = minusXbottomInd + y;

        minusXIndex[y*6 + 3] = minusXbottomInd + y + 1;
        minusXIndex[y*6 + 4] = minusXupInd + y + 1;
        minusXIndex[y*6 + 5] = minusXbottomInd + y;

        plusXIndex[y*6 + 0] = plusXupInd + y;
        plusXIndex[y*6 + 1] = plusXupInd + y + 1;
        plusXIndex[y*6 + 2] = plusXbottomInd + y;
        plusXIndex[y*6 + 4] = plusXbottomInd + y + 1;
        plusXIndex[y*6 + 3] = plusXupInd + y +1;
        plusXIndex[y*6 + 5] = plusXbottomInd + y;
    }
    ibo->setData(QByteArray(reinterpret_cast<const char*>(indexBuffer.data()), static_cast<int>(indexBuffer.size() * sizeof(unsigned int))));
    vbo->setData(QByteArray(reinterpret_cast<const char*>(buffer.data()), static_cast<int>(buffer.size() * sizeof(vertex))));

    auto indices = static_cast<unsigned int>(indexBuffer.size());
    vertices= static_cast<unsigned int>(buffer.size());
    indexAttr->setCount(indices);
    positionAttr->setCount(vertices);
    uvAttr->setCount(vertices);
    normalAttr->setCount(vertices);

    modified = false;
    emit modifiedChanged();
}

bool Material::isModified() const {
    return modified;
}
