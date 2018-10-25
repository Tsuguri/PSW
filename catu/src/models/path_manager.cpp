#include "path_manager.hpp"
#include <gcode/codes.hpp>
#include <QUrl>
#include <QMessageBox>


PathManager::PathManager(): _valid(false), _paths(), _toolManager(nullptr){

}


std::pair<float, MillType> CheckMillType(const std::string& fileName){
    auto place = fileName.find_last_of('.');
    if(place == 0 || place >= fileName.size()){
        return {10, MillType::Unknown};
    }
    auto extension = fileName.substr(place +1, fileName.size() -place);
    MillType type;
    if(extension[0] != 'k' && extension[0]!='f'){
        std::cout<<"Unknown mill type: "<< extension[0]<<std::endl;
        return {10, MillType::Unknown};
    }
    if(extension[0] == 'k'){
        type = MillType::Spheric;
    } else {
        type = MillType::Flat;
    }
    float radius = 0;
    try{
        radius = std::stoi(extension.substr(1, extension.size()-1));
    } catch(...){
        std::cout<<"Bad mill radius: "<<extension.substr(1, extension.size()-1)<<std::endl;
        return {10, MillType::Unknown};
    }

    return {radius, type};
}

    void PathManager::AddPath(QString filePath){
        // read tool from name
        //
        std::cout<<"Adding path..."<<std::endl;
        auto name = QUrl(filePath).path().toStdString();
        auto gcodes = GCode::Loader::Load(name);
        if(gcodes.size()==0){
            std::cout<<"No gcodes"<<std::endl;
            QMessageBox::warning(nullptr, "Bad file", "File is in bad format and coulnd't be loaded");
            return;
        }
        float radius;
        MillType type;
        std::tie(radius, type) = CheckMillType(name);
        auto tool = _toolManager->getTool(radius, type);
        if(tool == nullptr){
            std::cout<<"No tool"<<std::endl;
            return;
        }
        appendPath(new Path(filePath, std::move(gcodes),tool));
    }
void PathManager::RemovePath(int path){
    if(path < _paths.size()){
        auto ptr = _paths[path];
        _paths.erase(_paths.begin()+path);
        ptr->deleteLater();
        emit pathsChanged();
    }
}
void PathManager::setToolManager(ToolManager* manager){
    _toolManager=manager;
}

QQmlListProperty<Path> PathManager::getPaths(){
    return QQmlListProperty<Path>(this, this, &PathManager::appendPath, &PathManager::pathsCount, &PathManager::path, &PathManager::clearPaths);
}

void PathManager::appendPath(Path* path){
    _paths.push_back(path);
    std::cout<<"there are "<<_paths.size()<<" paths"<<std::endl;
    emit pathsChanged();
}

int PathManager::pathsCount()const {
    return _paths.size();
}

Path* PathManager::path(int id) const{
    return _paths[id];
}

void PathManager::clearPaths() {
    for(auto* elem: _paths){
        elem->deleteLater();
    }
    _paths.clear();
    emit pathsChanged();
}

    void PathManager::appendPath(QQmlListProperty<Path>* prop, Path* path){
        reinterpret_cast<PathManager*>(prop->data)->appendPath(path);
    }
    int PathManager::pathsCount(QQmlListProperty<Path>* prop){
        return reinterpret_cast<PathManager*>(prop->data)->pathsCount();
    }

    Path* PathManager::path(QQmlListProperty<Path>* prop, int id){
        return reinterpret_cast<PathManager*>(prop->data)->path(id);
    }

    void PathManager::clearPaths(QQmlListProperty<Path>* prop){
        reinterpret_cast<PathManager*>(prop->data)->clearPaths();
    }
