#include <QApplication>
#include <QQmlApplicationEngine>

#include "simulation.hpp"
#include "trace.hpp"


int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<Simulation>("Nigari", 1, 0, "Simulation");
  qmlRegisterType<Trace>("Nigari", 1, 0, "Trace");

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
