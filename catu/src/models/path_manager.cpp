#include "path_manager.hpp"
#include <gcode/codes.hpp>
#include <QUrl>


PathManager::PathManager(): _valid(false), _paths(), _toolManager(nullptr){

}


std::pair<float, MillType> CheckMillType(const std::string& fileName){
    auto place = fileName.find_last_of('.');
    if(place == 0 || place >= fileName.size()){
        return {10, MillType::Unknown};
    }
    auto extension = fileName.substr(place, fileName.size() -place);
    std::cout<<extension<<std::endl;
    MillType type;
    if(extension[0] != 'k' && extension[0]!='f'){
        return {10, MillType::Unknown};
    }
    if(extension[0] == 'k'){
        type = MillType::Spheric;
    } else {
        type = MillType::Flat;
    }
    float radius = 0;
    try{
        radius = std::stof(extension.substr(1, extension.size()-1));
    } catch(...){
        return {10, MillType::Unknown};
    }

    return {radius, type};
}

    void PathManager::AddPath(QString filePath){
        // read tool from name
        //
        auto name = QUrl(filePath).path().toStdString();
        auto gcodes = GCode::Loader::Load(name);
        if(gcodes.size()==0){
            return;
        }
        float radius;
        MillType type;
        std::tie(radius, type) = CheckMillType(name);
        auto tool = _toolManager->getTool(radius, type);
        if(tool == nullptr){
            return;
        }
        appendPath(new Path(filePath, std::move(gcodes),nullptr));
    }
void PathManager::RemovePath(Path* path){
    _paths.erase(std::remove(_paths.begin(), _paths.end(), path));
    emit pathsChanged();
}
void PathManager::setToolManager(ToolManager* manager){
    
}

QQmlListProperty<Path> PathManager::getPaths(){
    return QQmlListProperty<Path>(this, this, &PathManager::appendPath, &PathManager::pathsCount, &PathManager::path, &PathManager::clearPaths);
}

void PathManager::appendPath(Path* path){
    _paths.push_back(path);
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
