#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "CSpaceImageProvider.hpp"
#include "simulation.hpp"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<Simulation>("Kata", 1, 0, "Simulation");
  qmlRegisterType<Box>("Kata", 1, 0, "Box");

  CSpaceImageProvider* imageProvider = new CSpaceImageProvider();

  engine.addImageProvider("cSpaceImagesProvider", imageProvider);
  engine.rootContext()->setContextProperty("cSpaceImagesProvider",
                                           imageProvider);

  engine.load(QUrl("qrc:/main.qml"));
  if (engine.rootObjects().isEmpty()) return -1;

  return app.exec();
}
