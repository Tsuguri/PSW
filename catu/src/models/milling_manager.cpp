#include <models/milling_manager.hpp>


MillingManager::MillingManager(QObject* parent) : QObject(parent), _running(false), _active(false), _tools(nullptr), _paths(nullptr){}


void MillingManager::Run() {
    if(!_active && _running) {
        _active = true;
    } else {
        _running = true;
        _active = true;
    emit runningChanged();
    }
        emit activeChanged();
}

void MillingManager::Pause() {
    _active = false;
    emit activeChanged();
}

void MillingManager::Stop() {
    _active = false;
    _running = false;
    emit activeChanged();
    emit runningChanged();
}

bool MillingManager::isRunning() const {
    return _running;
}

bool MillingManager::isActive() const {
    return _active;
}

void MillingManager::setTools(ToolManager* manager) {
    _tools = manager;
}

void MillingManager::setPaths(PathManager* manager) {
    _paths = manager;
}
