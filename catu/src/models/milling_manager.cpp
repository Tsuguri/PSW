#include <models/milling_manager.hpp>
#include <cmath>
#include <QThreadPool>
#include <QRunnable>


MillingManager::MillingManager(QObject* parent)
    : QObject(parent),
    _running(false),
    _active(false),
    _speed(10),
    _progress(0),
    _currentPos(),
    _totalLength(0),
    _tools(nullptr),
    _downsideFlatMod(false),
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
        _downsideFlatMod = false;
        emit downsideMill();
        _currentData.currentCommand=1;
        _currentPos = _paths->path(0)->getCommand(0)->MoveFrom(vec3{});
        _currentData.currentPath=0;
        _currentData.paths=_paths;
        _currentData.lengthDoneInCurrent =0;

        _totalLength=0;
        for(int i=0; i<_paths->pathsCount(); i++) {
        _totalLength += _paths->path(i)->points();
        }

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
    _material->updateNormals();
    _material->sendVertices();
    emit activeChanged();
    emit runningChanged();
}

class ToEnd : public QRunnable {
    void run() override {
        mgr->Finish();

    }
    public:
    ToEnd(MillingManager* mgr) : mgr(mgr) {}

    private:
    MillingManager* mgr;
};

void MillingManager::Finish() {

    auto& d = _currentData;

    while(true){
        auto mill = d.paths->path(d.currentPath)->getTool();
        auto comm = d.paths->path(d.currentPath)->getCommand(d.currentCommand);
        auto to = comm->MoveFrom(_currentPos);
        _progress+=1;
        emit progressChanged();
        PerformStep(_currentPos, to, mill, false);
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

    _material->updateNormals();
    _material->sendVertices(false);
}

void MillingManager::RunToEnd() {

    _timer->stop();
    QThreadPool::globalInstance()->start(new ToEnd(this));
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
    return _progress/_totalLength;
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

    if( _material->mill(tool, from, to, updateBuffer) && (to.z < from.z-0.0001)&& tool->type()==MillType::Flat) {
        if(!_downsideFlatMod){
            _downsideFlatMod=true;
            emit downsideMill();
        }
    }
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

    float x1 = _currentPos.x;
    float x2 = _currentPos.x;
    float y1 = _currentPos.y;
    float y2 = _currentPos.y;
    while(timeToRun > 0) {
        auto canMove = timeToRun * _speed * 10;
        auto mill = d.paths->path(d.currentPath)->getTool();
        radius = std::max(radius, (float)mill->getRadius());
        auto comm = d.paths->path(d.currentPath)->getCommand(d.currentCommand);
        auto to = comm->MoveFrom(_currentPos);
        x1 = std::min(x1, to.x);
        x2 = std::max(x2, to.x);
        y1 = std::min(y1, to.y);
        y2 = std::max(y2, to.y);
        auto movement = vec3(to.x-_currentPos.x, to.y-_currentPos.y, to.z-_currentPos.z);
        auto len = movement.len();
        if(len > canMove) {
            auto moveTo = Interpolate(_currentPos, to, canMove/len);
            PerformStep(_currentPos, moveTo, mill);
            timeToRun = 0;
        } else {
            PerformStep(_currentPos, to, mill);
            _progress+=1;
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
    _material->updateNormals(x1 - radius/2.0,x2+radius/2.0, y1-radius/2.0, y2+radius/2.0);
    _material->sendVertices();
    emit toolPosChanged();
    emit progressChanged();

}
