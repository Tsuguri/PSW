#pragma once

#include<QObject>

enum class MillType{
    Spheric,
    Flat,
    Unknown
};
class Mill : public QObject {
    Q_OBJECT

    Q_PROPERTY(int toolRadius READ getRadius CONSTANT);
    Q_PROPERTY(QString type READ getType CONSTANT);
    Q_PROPERTY(bool inUse READ isUsed NOTIFY usedChanged);

    public:
    explicit Mill();
    explicit Mill(int radius, MillType type);
    int getRadius()const;
    QString getType() const;
    bool isUsed() const;

    void addUsingPath();
    void removeUsingPath();

    MillType type() const;
signals:
    void usedChanged();

    private:
    int _radius;
    MillType _type;
    int _paths;
};
