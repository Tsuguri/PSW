#include <models/tool_manager.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <QMessageBox>

ToolManager::ToolManager(): QObject(nullptr),_mills() {}

Mill* ToolManager::getTool(int radius, MillType type){
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
            emit toolsChanged();
            return newMill;

        }
    } else {
        return *tool;
    }

    return nullptr;
}

QQmlListProperty<Mill> ToolManager::getTools() {
    return QQmlListProperty<Mill>(this, this, &ToolManager::appendTool, &ToolManager::toolsCount, &ToolManager::tool, &ToolManager::clearTools);
}

void ToolManager::AddTool(int radius, short type) {
    auto toolType = type==0 ? MillType::Spheric : MillType::Flat;
    auto tool = new Mill(radius, toolType);
    _mills.push_back(tool);
    emit toolsChanged();
}

void ToolManager::RemoveTool(int index) {
    if(index < 0 || index>=_mills.size()){
        return;
    }
    _mills[index]->deleteLater();
    _mills.erase(_mills.begin()+index);
    emit toolsChanged();
}

void ToolManager::appendTool(Mill* mill) {
    _mills.push_back(mill);
    emit toolsChanged();
}

int ToolManager::toolsCount() const {
    return _mills.size();
}

Mill* ToolManager::tool(int id) const {
    return _mills[id];
}

void ToolManager::clearTools() {
    for(auto* elem: _mills){
        elem->deleteLater();
    }
    _mills.clear();
    emit toolsChanged();
}

void ToolManager::appendTool(QQmlListProperty<Mill>* prop, Mill* mill) {
    reinterpret_cast<ToolManager*>(prop->data)->appendTool(mill);
}

int ToolManager::toolsCount(QQmlListProperty<Mill>* prop) {
    return reinterpret_cast<ToolManager*>(prop->data)->toolsCount();
}

Mill* ToolManager::tool(QQmlListProperty<Mill>* prop, int index) {
    return reinterpret_cast<ToolManager*>(prop->data)->tool(index);
}

void ToolManager::clearTools(QQmlListProperty<Mill>* prop){

    reinterpret_cast<ToolManager*>(prop->data)->clearTools();
}
