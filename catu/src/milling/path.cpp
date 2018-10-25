#include <milling/path.hpp>

Path::Path(QObject* parent): QObject(parent) {
    _tool = new Mill();
}

Path::Path(QString str, GCode::Program program, Mill* mill) : _file(str), _tool(mill), _program(std::move(program)){
}

QString Path::getFileName() const {
    return _file;
}

Mill* Path::getTool() const {
    return _tool;
}
