#include <QApplication>
#include <QQmlApplicationEngine>

#include "BezierFrameGeometry.hpp"
#include "CubeWalls.hpp"
#include "CuboidWireframeGeometry.hpp"
#include "ParticleModel.hpp"
#include "QmlRaycast.hpp"
#include "simulation.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //qmlRegisterType<Simulation>("Hoken", 1, 0, "Simulation");

    //qmlRegisterSingletonType<QQuaternionUtils>("Hoken", 1, 0, "Quat", QQuaternionUtilsSingletonProvider);
    qmlRegisterType<CuboidWireframeGeometry>("Geru", 1, 0, "CuboidWireframeGeometry");
    qmlRegisterType<BezierFrameGeometry>("Geru", 1, 0, "BezierFrameGeometry");
    //qmlRegisterType<ParticleModel>("Geru", 1, 0, "ParticleModel");
    qmlRegisterType<Simulation>("Geru", 1, 0, "Simulation");
    qmlRegisterType<Spring>("Geru", 1, 0, "Spring");
    qmlRegisterType<Point>("Geru", 1, 0, "Point");
    qmlRegisterType<CubeWalls>("Geru", 1, 0, "CubeWalls");
    qmlRegisterType<QmlRaycast>("fhac.mascor", 1, 0, "Raycast");

    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
