#include <QApplication>
#include <QQmlApplicationEngine>

#include "simulation.hpp"
#include "QQuaternionUtils.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<Simulation>("Hoken", 1, 0, "Simulation");

  qmlRegisterSingletonType<QQuaternionUtils>("Hoken", 1, 0, "Quat", QQuaternionUtilsSingletonProvider);

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
