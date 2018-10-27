#pragma once

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include <models/tool_manager.hpp>
#include <models/path_manager.hpp>
#include <material.hpp>
#include <QVector3D>

struct WorkData {
    PathManager* paths;
    size_t currentPath;
    size_t currentCommand;
    float lengthDoneInCurrent;
};

class MillingManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged);
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged);
    // in cm/s
    Q_PROPERTY(float speed READ getSpeed WRITE setSpeed NOTIFY speedChanged);
    Q_PROPERTY(float progress READ getProgress NOTIFY progressChanged);
    Q_PROPERTY(float totalLength READ getTotalLength NOTIFY totalLengthChanged);
    Q_PROPERTY(QVector3D toolpos READ getToolPos NOTIFY toolPosChanged);
    Q_PROPERTY(ToolManager* tools WRITE setTools);
    Q_PROPERTY(PathManager* paths WRITE setPaths);
    Q_PROPERTY(Material* material WRITE setMaterial);

    public:
    constexpr static int updateInterval = 10;

    explicit MillingManager(QObject* parent = nullptr);

    Q_INVOKABLE void Run();
    Q_INVOKABLE void Pause();
    Q_INVOKABLE void Stop();

    bool isRunning() const;
    bool isActive() const;
    float getSpeed() const;
    float getProgress() const;
    float getTotalLength() const;
    QVector3D getToolPos() const;

    void setSpeed(float speed);
    void setTools(ToolManager* manager);
    void setPaths(PathManager* manager);
    void setMaterial(Material* material);
signals:
    void runningChanged();
    void activeChanged();
    void speedChanged();
    void progressChanged();
    void totalLengthChanged();
    void toolPosChanged();

    private:
    void setProgress(float progress);
    void PerformStep(vec3 from, vec3 to, Mill* tool, bool updateBuffer = true);

    void timerTick();
    bool _running;
    bool _active;
    float _speed;
    float _progress;
    vec3 _currentPos;

    float _totalLength;

    QTimer* _timer;
    QElapsedTimer* _elapsed;

    ToolManager* _tools;
    PathManager* _paths;
    Material* _material;

    WorkData _currentData;
};
