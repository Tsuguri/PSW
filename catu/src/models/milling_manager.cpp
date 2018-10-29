#include <models/milling_manager.hpp>
#include <cmath>
#include <QThread>


MillingManager::MillingManager(QObject* parent)
    : QObject(parent),
    _running(false),
    _active(false),
    _speed(10),
    _progress(0),
    _currentPos(),
    _totalLength(0),
    _tools(nullptr),
    _paths(nullptr),
    _timer(new QTimer(this)),
    _elapsed(new QElapsedTimer()) {
    _timer->setInterval(updateInterval);
    connect(_timer, &QTimer::timeout, this, &MillingManager::timerTick);

    _elapsed = new QElapsedTimer();

}


void MillingManager::Run() {
    if(!_active && _running) {
        _active = true;
    } else {
        if(_paths->pathsCount() == 0) {
            std::cout<<"No paths"<<std::endl;
            return;
        }
        _running = true;
        _active = true;
        _progress = 0;
        _currentData.currentCommand=1;
        _currentPos = _paths->path(0)->getCommand(0)->MoveFrom(vec3{});
        _currentData.currentPath=0;
        _currentData.paths=_paths;
        _currentData.lengthDoneInCurrent =0;
        _timer->start();
        _elapsed->start();
        emit progressChanged();
        emit runningChanged();
    }
    emit activeChanged();
}

void MillingManager::Pause() {
    _active = false;
    _material->updateNormals();
    _material->sendVertices();
    emit activeChanged();
}

void MillingManager::Stop() {
    _active = false;
    _running = false;
    _timer->stop();
    emit activeChanged();
    emit runningChanged();
}

bool MillingManager::isRunning() const {
    return _running;
}

bool MillingManager::isActive() const {
    return _active;
}

float MillingManager::getSpeed() const {
    return _speed;
}

float MillingManager::getProgress() const {
    return _progress;
}

float MillingManager::getTotalLength() const {
    return _totalLength;
}

QVector3D MillingManager::getToolPos() const {
    return QVector3D(_currentPos.x, _currentPos.z, _currentPos.y);
}

void MillingManager::setSpeed(float speed) {
    _speed = speed;
}

void MillingManager::setTools(ToolManager* manager) {
    _tools = manager;
}

void MillingManager::setPaths(PathManager* manager) {
    _paths = manager;
}

void MillingManager::setMaterial(Material* material) {
    _material = material;
}

void MillingManager::setProgress(float progress) {
    _progress = progress;
    emit progressChanged();
}

void MillingManager::PerformStep(vec3 from, vec3 to, Mill* tool, bool updateBuffer ){

    _material->mill(tool, from, to, updateBuffer);
    _currentPos = to;
}
void MillingManager::timerTick(){
    auto time = _elapsed->restart();

    if(!_active) {
        return;
    }

    float timeToRun = time/1000.0; // w sekundach
    auto& d = _currentData;
    auto from = _currentPos;

    float radius = 0;
    while(timeToRun > 0) {
        auto canMove = timeToRun * _speed * 10;
        auto mill = d.paths->path(d.currentPath)->getTool();
        radius = std::max(radius, (float)mill->getRadius());
        auto comm = d.paths->path(d.currentPath)->getCommand(d.currentCommand);
        auto to = comm->MoveFrom(_currentPos);
        auto movement = vec3(to.x-_currentPos.x, to.y-_currentPos.y, to.z-_currentPos.z);
        auto len = movement.len();
        if(len > canMove) {
            auto moveTo = Interpolate(_currentPos, to, canMove/len);
            PerformStep(_currentPos, moveTo, mill);
            timeToRun = 0;
        } else {
            PerformStep(_currentPos, to, mill);
            timeToRun -=len/(_speed*10);
            d.currentCommand++;
            if(d.currentCommand >= d.paths->path(d.currentPath)->points()) {
                d.currentCommand=1;
                d.currentPath++;
                if(d.currentPath >= d.paths->pathsCount()) {
                    _material->updateNormals();
                    _running = _active = false;
                    _timer->stop();
                    emit runningChanged();
                    emit activeChanged();
                    break;
                }
                _currentPos = d.paths->path(d.currentPath)->getCommand(0)->MoveFrom(vec3{});
            }

        }
    }
    _material->updateNormals(from.x - radius/2.0,_currentPos.x+radius/2.0, from.y-radius/2.0, _currentPos.y+radius/2.0);
    _material->sendVertices();
    emit toolPosChanged();

}
