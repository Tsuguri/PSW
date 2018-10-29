#include <material.hpp>
#include <iostream>

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QDiffuseMapMaterial>

#include <QTextureImage>

Material::Material(Qt3DCore::QEntity *parent) : Qt3DCore::QEntity(parent), modified(false), tempArray() {
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
  resize(150.0f, 150.0f, 50.0f, 1000, 1000);
}

void Material::resetHeights() {

    auto xp = xRes+1;
    auto yp = yRes+1;
    for(unsigned int x=0;x<xp;x++) {
        for(unsigned int y=0;y<yp;y++) {
            buffer[y*xp+x].position.z = gridSize.z;
        }
        minusYup[x].position.z = gridSize.z;
        plusYup[x].position.z = gridSize.z;

    }
    for(unsigned int y =0; y<yp; y++) {
        minusXup[y].position.z = gridSize.z;
        plusXup[y].position.z = gridSize.z;
    }
    updateNormals();

    sendVertices();

}
void Material::resize(float xSize, float ySize, float height, unsigned int xResolution, unsigned int yResolution){
    this->gridSize = vec3{xSize, ySize, height};
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
            vert.position = vec3{xFrom + x*xd, yFrom + y*yd, height};
            vert.normal = vec3{0.0f, 0.0f, 1.0f};
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
        minusYbottom[x].position = vec3{xPos, yFrom, 0};
        minusYbottom[x].normal = vec3{0, -1, 0};
        minusYbottom[x].texCoord = vec2{x*xd/gridSize.x, 0};
        minusYup[x].position = vec3{xPos, yFrom, height};
        minusYup[x].normal = vec3{0, -1, 0};
        minusYup[x].texCoord = vec2{x*xd/gridSize.x, 1};
        plusYbottom[x].position = vec3{xPos, -yFrom, 0};
        plusYbottom[x].normal = vec3{0, 1, 0};
        plusYbottom[x].texCoord = vec2{x*xd/gridSize.x, 0};
        plusYup[x].position = vec3{xPos, -yFrom, height};
        plusYup[x].normal = vec3{0, 1, 0};
        plusYup[x].texCoord = vec2{x*xd/gridSize.x, 1};
    }

    for(unsigned int y = 0; y<yVerts; y++){
        auto yPos = yFrom + y*yd;
        minusXbottom[y].position =vec3{xFrom, yPos, 0};
        minusXbottom[y].normal = vec3{-1, 0, 0};
        minusXbottom[y].texCoord = vec2{y*yd/gridSize.y, 0};
        minusXup[y].position =vec3{xFrom, yPos, height};
        minusXup[y].normal = vec3{-1, 0, 0};
        minusXup[y].texCoord = vec2{y*yd/gridSize.y, 1};
        plusXbottom[y].position =vec3{-xFrom, yPos, 0};
        plusXbottom[y].normal = vec3{1, 0, 0};
        plusXbottom[y].texCoord = vec2{y*yd/gridSize.y, 0};
        plusXup[y].position =vec3{-xFrom, yPos, height};
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
    tempArray = QByteArray(reinterpret_cast<const char*>(buffer.data()), static_cast<int>(buffer.size() * sizeof(vertex)));
    vbo->setData(tempArray);

    auto indices = static_cast<unsigned int>(indexBuffer.size());
    vertices= static_cast<unsigned int>(buffer.size());
    indexAttr->setCount(indices);
    positionAttr->setCount(vertices);
    uvAttr->setCount(vertices);
    normalAttr->setCount(vertices);

    modified = false;
    emit modifiedChanged();
}

void Material::sendVertices() {
    //if(modified)
    //
    //std::cout<<"empty: "<<tempArray.isEmpty() <<" "<<tempArray.isNull()<<" "<<tempArray.size()<<std::endl;
    auto dat = reinterpret_cast<const char*>(buffer.data());
    auto size = static_cast<unsigned int>(buffer.size() * sizeof(vertex));
    //std::cout<<"vertices size: "<<size<<std::endl;
    auto destination = tempArray.data();
    for(unsigned int i =0; i<size; i++){
        //std::cout<<i<<std::endl;
        destination[i] = dat[i];
    }
    //std::cout<<"heho"<<std::endl;

    vbo->setData(tempArray);
}

vec3 Material::trans(vec3 pos) {
    pos.x +=gridSize.x/2.0f;
    pos.y +=gridSize.y/2.0f;

    return pos;
    
}

void Material::setHeight(int x, int y, float height){

    // dodać frezowanie krawędzi
    auto h =buffer[y*(xRes+1)+x].position.z;
    h = std::min(h, height);
    //std::cout<<"h: "<<h<<std::endl;
    buffer[y*(xRes+1)+x].position.z = h;

    if(x==0){
        minusXup[y].position.z = h;
        //minusXup[y].texCoord.v = h/gridSize.z;
    } else if (x== xRes+1){
        plusXup[y].position.z = h;
        //plusXup[y].texCoord.v = h/gridSize.z;
    
    }

    if(y==0){
        minusYup[x].position.z = h;
        minusYup[x].texCoord.v = h/gridSize.z;
    } else if (y== yRes){
        //std::cout<<"from "<<plusYup[x].position.z<<" to "<<h<<std::endl;
        plusYup[x].position.z = h;
        //plusYup[x].texCoord.v = h/gridSize.z;
    
    }
}

void Material::updateNormal(int x, int y) {
    vec3 normal{};
    auto negX = x>0;
    auto posX = x<xRes;
    auto negY = y>0;
    auto posY = y<yRes;

    auto pos = buffer[y*(xRes+1)+x].position;

    if(negX && negY){
        normal= normal + Cross((buffer[(y-1)*(xRes+1)+(x)].position - pos).normalized(), 
                               (buffer[(y)*(xRes+1) + (x -1)].position-pos).normalized());
    }
    if(negX && posY){
        normal= normal + Cross((buffer[(y)*(xRes+1)+(x-1)].position - pos).normalized(), 
                               (buffer[(y+1)*(xRes+1) + (x)].position-pos).normalized());
    }
    if(posX && posY){
        normal= normal + Cross((buffer[(y+1)*(xRes+1)+(x)].position - pos).normalized(), 
                               (buffer[(y)*(xRes+1) + (x +1)].position-pos).normalized());
    }
    if(posX && negY){
        normal= normal + Cross((buffer[(y)*(xRes+1)+(x+1)].position - pos).normalized(), 
                               (buffer[(y-1)*(xRes+1) + (x)].position-pos).normalized());
    }
    normal = vec3{} - normal.normalized();
    buffer[y*(xRes+1)+x].normal = normal;

}

void Material::updateNormals() {
    for(int x = 0; x<xRes+1; x++){
        for(int y = 0; y<yRes+1; y++){
            updateNormal(x,y);
        }
    }

}

void Material::updateNormals(float xFrom, float xTo, float yFrom, float yTo){

    auto dx = gridSize.x / (xRes+1);
    auto dy = gridSize.y / (yRes+1);
    auto materialSpace = trans(vec3{xFrom, yFrom, 0});
    auto materialSpaceTo = trans(vec3{xTo, yTo, 0});
    int fx = materialSpace.x/dx;
    int tx = materialSpaceTo.x/dx;
    int fy = materialSpace.y/dy;
    int ty = materialSpaceTo.y/dy;
    

    for(int x = fx; x<=tx; x++) {
        for(int y = fy; y<=ty; y++) {
            if(x<0 || y<0 || x>xRes+1 || y>yRes+1) {
                //std::cout<<"discarded "<<x<<" "<<y<<std::endl;
                continue;
            }
            updateNormal(x,y);
        }
    }
}

void Material::millPlace(vec3 pos, float radius, const std::function<float(float, float)>& heightLamb) {
    auto materialSpace = trans(pos);
    if(materialSpace.x + radius < 0 || materialSpace.x - radius > gridSize.x || materialSpace.y + radius <0 || materialSpace.y - radius > gridSize.y){
        //mill outside of material, no milling
        return;
    }
    auto dx = gridSize.x / (xRes+1);
    auto dy = gridSize.y / (yRes+1);
    int rdx = radius/dx; // number of grains in radius along x axis;
    int rdy = radius/dy; // number of grains in radius along y axis;
    int px = materialSpace.x / dx;
    int py = materialSpace.y / dy;
    //std::cout<<"milling at: "<<px<<" "<<py<<" rdx "<<rdx<<" rdy "<<rdy<<std::endl;
    float r2 = radius*radius;

    for(int x = px - rdx; x<=px+rdx; x++) {
        for(int y = py - rdy; y<=py+rdy; y++) {
            if(x<0 || y<0 || x>xRes+1 || y>yRes+1) {
                //std::cout<<"discarded "<<x<<" "<<y<<std::endl;
                continue;
            }
            // mill if in radius.
            auto distx = std::abs(x-px)* dx;
            auto disty = std::abs(y-py)* dy;
            auto dist2 = distx*distx + disty*disty;
            if(dist2< r2) {
                auto h = heightLamb(std::sqrt(dist2), pos.z);
                setHeight(x,y, h);
            }
                
        }
    }
    


}

void Material::mill(Mill* tool, vec3 from, vec3 to, bool updateBuffer) {
    auto radius = tool->getRadius();
    std::function<float(float, float)> lamb;
    if (tool->type() == MillType::Flat){
        lamb = [radius](float distance, float h) { return h;};
    } else {
        lamb = [radius](float distance, float h) {return h+radius-std::sqrt(radius*radius - distance*distance);};
    }

    auto vect = to-from;
    auto dist = vect.len();
    auto len = tool->getRadius()/10;
    auto steps = dist/len;
    for(unsigned int i=0;i<steps; i++) {
        auto pos = Interpolate(from, to, i*len/dist);
        millPlace(pos, tool->getRadius()/2, lamb);
    }

    //millPlace(from, tool->getRadius()/2, lamb);


    //std::cout<<"tool "<<tool->getRadius()<<" "<<tool->getType().toStdString()<<" to: "<<to.x<<" "<<to.y<<" "<<to.z<<std::endl;
    //updateNormals();
}

bool Material::isModified() const {
    return modified;
}
