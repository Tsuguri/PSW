#include <milling/path.hpp>

Path::Path(QObject* parent): QObject(parent), _file("nothing"), _program(), _length(0) {
    _tool = new Mill();
}

Path::Path(QString str, GCode::Program program, Mill* mill) : _file(str), _tool(mill), _program(std::move(program)){
    _tool->addUsingPath();
    _length = 0.0f;
    for(const auto& elem: _program){
        _length += elem->GetLength();
    }
}

Path::~Path() {
    _tool ->removeUsingPath();
}

QString Path::getFileName() const {
    return _file;
}

Mill* Path::getTool() const {
    return _tool;
}

float Path::getLength() const {
    return _length;
}

unsigned int Path::points() const {
    return _program.size();
}

void Path::enumeratePositions(pathDelegate lamb) const{
    vec3 pos = {0, 0, 0};
    for(const auto& elem : _program) {
        auto mv = elem->GetMovement();
        if(mv.x)
            pos.x = *mv.x;
        if(mv.y)
            pos.y=*mv.y;
        if(mv.z)
            pos.z=*mv.z;
        lamb(pos);
    }
}

GCode::Command* Path::getCommand(unsigned int id) const {
    return _program[id].get();
}
