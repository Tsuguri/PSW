#include <QApplication>
#include <QQmlApplicationEngine>

#include "simulation.hpp"
#include "math.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  // qmlRegisterType<PathfindingSimulator>("PSW", 1, 0, "Simulator");
  qmlRegisterType<MathFormula>("Haru", 1, 0, "MathFormula");
  qmlRegisterType<Simulation>("Haru", 1, 0, "Simulation");

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
