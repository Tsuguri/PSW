#include <tools/mill.hpp>


Mill::Mill(): _radius(10), _type(MillType::Spheric){}
Mill::Mill(float radius, MillType type): _radius(radius), _type(type){}

    float Mill::getRadius()const{
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
