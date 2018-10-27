#pragma once

#include <gcode/codes.hpp>
#include <tools/mill.hpp>
#include <QObject>
#include <functional>

using pathDelegate = std::function<void(vec3)>;

class Path: public QObject{
    Q_OBJECT

    Q_PROPERTY(QString file READ getFileName CONSTANT)
    Q_PROPERTY(Mill* tool READ getTool CONSTANT)
    Q_PROPERTY(float pathLength READ getLength CONSTANT)

    public:
    explicit Path(QObject* parent = nullptr);
    explicit Path(QString str, GCode::Program program,  Mill* tool);
    ~Path();

    void enumeratePositions(pathDelegate lamb) const;

        QString getFileName() const;
        Mill* getTool() const;
        float getLength() const;
        unsigned int points() const;

    private:
        QString _file;
        Mill* _tool;
        GCode::Program _program;
        float _length;
};
