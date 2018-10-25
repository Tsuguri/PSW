#pragma once

#include <gcode/codes.hpp>
#include <tools/mill.hpp>
#include <QObject>

class Path: public QObject{
    Q_OBJECT

    Q_PROPERTY(QString file READ getFileName CONSTANT)
    Q_PROPERTY(Mill* tool READ getTool CONSTANT)

    public:
    explicit Path(QObject* parent = nullptr);
    explicit Path(QString str, GCode::Program program,  Mill* tool);

        QString getFileName() const;
        Mill* getTool() const;

    private:
        QString _file = "heheszki";
        Mill* _tool = nullptr;
        GCode::Program _program;
};
