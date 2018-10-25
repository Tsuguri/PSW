#include "tool_manager.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <QMessageBox>

ToolManager::ToolManager(): QObject(nullptr),_mills() {}

Mill* ToolManager::getTool(float radius, MillType type){
    std::cout<<"Looking for tool with r: "<<radius<<" and type: "<<(short)type<<std::endl;
    auto tool = std::find_if(_mills.begin(), _mills.end(), [radius, type](const auto& elem) {
            return elem->type()==type && elem->getRadius()==radius;
        });
    if(tool==_mills.end()){
        std::stringstream msg;
        msg<<" Do you want to add this tool(radius: "<<radius<<" type: "<<(type==MillType::Spheric ? "Spheric" : "Flat")<<"), or cancel adding this path?";
        auto ret = QMessageBox::question(nullptr, "Tool not found.", msg.str().c_str(), QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == QMessageBox::Ok){
            auto newMill = new Mill(radius, type);
            _mills.push_back(newMill);
            return newMill;

        }
    } else {
        return *tool;
    }

    return nullptr;
}
