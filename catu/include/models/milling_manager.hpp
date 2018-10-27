#pragma once

#include <QObject>

#include <models/tool_manager.hpp>
#include <models/path_manager.hpp>


class MillingManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged);
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged);
    Q_PROPERTY(ToolManager* tools WRITE setTools);
    Q_PROPERTY(PathManager* paths WRITE setPaths);

    public:
    explicit MillingManager(QObject* parent = nullptr);

    Q_INVOKABLE void Run();
    Q_INVOKABLE void Pause();
    Q_INVOKABLE void Stop();

    bool isRunning() const;
    bool isActive() const;

    void setTools(ToolManager* manager);
    void setPaths(PathManager* manager);
signals:
    void runningChanged();
    void activeChanged();

    private:
    bool _running;
    bool _active;

    ToolManager* _tools;
    PathManager* _paths;
};
