#pragma once

#include<QObject>

enum class MillType{
    Spheric,
    Flat,
    Unknown
};
class Mill : public QObject {
    Q_OBJECT

    Q_PROPERTY(float radius READ getRadius CONSTANT);
    Q_PROPERTY(QString type READ getType CONSTANT);

    public:
    explicit Mill();
    explicit Mill(float radius, MillType type);
    float getRadius()const;
    QString getType() const;

    MillType type() const;

    private:
    float _radius;
    MillType _type;
};
