#include <QApplication>
#include <QQmlApplicationEngine>

#include "material.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<Material>("Catu", 1, 0, "Material");

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
