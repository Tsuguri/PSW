#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //qmlRegisterType<PathfindingSimulator>("PSW", 1, 0, "Simulator");
    //qmlRegisterType<Formula>("PSW", 1, 0, "Formula");

    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
