#pragma once

#include<QObject>

#include <tools/mill.hpp>

class ToolManager : public QObject {
    Q_OBJECT

    public:
        explicit ToolManager();

        Mill* getTool(float radius, MillType type);


    private:
        std::vector<Mill*> _mills;
};
