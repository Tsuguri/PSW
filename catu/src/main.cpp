#include <QApplication>
#include <QQmlApplicationEngine>

#include <material.hpp>
#include "models/path_manager.hpp"
#include "models/milling_manager.hpp"
#include "models/tool_manager.hpp"
#include <tools/mill.hpp>
#include "models/path_render.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
QCoreApplication::setOrganizationName("QtExamples");
  QQmlApplicationEngine engine;

  qmlRegisterType<Material>("Catu", 1, 0, "Material");
  qmlRegisterType<PathRender>("Catu", 1, 0, "PathRender");
  qmlRegisterType<Path>("Catu", 1, 0, "Path");
  qmlRegisterType<Mill>("Catu", 1, 0, "Mill");
  qmlRegisterType<PathManager>("Catu", 1, 0, "PathManager");
  qmlRegisterType<ToolManager>("Catu", 1, 0, "ToolManager");
  qmlRegisterType<MillingManager>("Catu", 1, 0, "MillingManager");

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
