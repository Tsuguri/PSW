#include <QApplication>
#include <QQmlApplicationEngine>

#include "BezierFrameGeometry.hpp"
#include "CuboidWireframeGeometry.hpp"
#include "QmlRaycast.hpp"
#include "simulation.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<CuboidWireframeGeometry>("Geru", 1, 0, "CuboidWireframeGeometry");
    qmlRegisterType<BezierFrameGeometry>("Geru", 1, 0, "BezierFrameGeometry");
    qmlRegisterType<Simulation>("Geru", 1, 0, "Simulation");
    qmlRegisterType<Spring>("Geru", 1, 0, "Spring");
    qmlRegisterType<Point>("Geru", 1, 0, "Point");
    qmlRegisterType<QmlRaycast>("fhac.mascor", 1, 0, "Raycast");

    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
