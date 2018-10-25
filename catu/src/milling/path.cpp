#include <milling/path.hpp>

Path::Path(QObject* parent): QObject(parent), _file("nothing"), _program(), _length(0) {
    _tool = new Mill();
}

Path::Path(QString str, GCode::Program program, Mill* mill) : _file(str), _tool(mill), _program(std::move(program)){
    _length = 0.0f;
    for(const auto& elem: _program){
        _length += elem->GetLength();
    }
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
