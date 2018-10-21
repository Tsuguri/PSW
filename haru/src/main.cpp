#include <QApplication>
#include <QQmlApplicationEngine>

#include "simulation.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  // qmlRegisterType<PathfindingSimulator>("PSW", 1, 0, "Simulator");
  qmlRegisterType<Simulation>("Haru", 1, 0, "Simulation");

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
