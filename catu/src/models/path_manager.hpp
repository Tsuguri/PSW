#pragma once
#include <iostream>

#include<QObject>
#include <QQmlListProperty>

#include <milling/path.hpp>
#include "tool_manager.hpp"


class PathManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool valid READ getValid NOTIFY validChanged);
    Q_PROPERTY(QQmlListProperty<Path> paths READ getPaths NOTIFY pathsChanged);
    Q_PROPERTY(ToolManager* toolManager WRITE setToolManager);

    public:
    explicit PathManager();
    bool getValid() const {
        return _valid;
    }

    void setToolManager(ToolManager* manager);

    Q_INVOKABLE void toggle(){
        _valid = !_valid;
        emit validChanged();
    }

    Q_INVOKABLE void AddPath(QString filePath);
    Q_INVOKABLE void RemovePath(Path* path);
    QQmlListProperty<Path> getPaths();


    void appendPath(Path* path);
    int pathsCount() const;
    Path* path(int id) const;
    void clearPaths();

signals:
    void validChanged();
    void pathsChanged();


private:
    static void appendPath(QQmlListProperty<Path>* prop, Path*);
    static int pathsCount(QQmlListProperty<Path>* prop);
    static Path* path(QQmlListProperty<Path>* prop, int);
    static void clearPaths(QQmlListProperty<Path>* prop);

    bool _valid;
    std::vector<Path*> _paths;
    ToolManager* _toolManager;
};
