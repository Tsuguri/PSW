#include "QQuaternionUtils.hpp"

QQuaternionUtils::QQuaternionUtils(QObject *parent) : QObject(parent)
{

}

QObject *QQuaternionUtilsSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new QQuaternionUtils();
}
