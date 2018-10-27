#include <tools/mill.hpp>


Mill::Mill(): _radius(10), _type(MillType::Spheric), _paths(0){}
Mill::Mill(int radius, MillType type): _radius(radius), _type(type), _paths(0){}

int Mill::getRadius()const{
    return _radius;
}
QString Mill::getType() const{
    switch(_type){
        case MillType::Spheric:
            return "Spheric";
        case MillType::Flat:
            return "Flat";
    }
    return "Unknown";
}

MillType Mill::type() const {
    return _type;
}

bool Mill::isUsed() const {
    return _paths != 0;
}

void Mill::addUsingPath() {
    _paths++;
    usedChanged();
}

void Mill::removeUsingPath() {
    _paths--;
    usedChanged();
}
