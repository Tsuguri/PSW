#pragma once
#include <QObject>
#include <QQmlListProperty>
#include <QVector3D>

class Point : public QObject {
    Q_OBJECT
  public:
    explicit Point(QObject* parent = nullptr);

    Q_PROPERTY(QVector3D position READ getPos WRITE setPos NOTIFY posChanged);
    Q_PROPERTY(QVector3D velocity READ getVel WRITE setVel NOTIFY velChanged);
    Q_PROPERTY(QVector3D acceleration READ getAcc WRITE setAcc NOTIFY accChanged);

    QVector3D getPos() const;
    QVector3D getVel() const;
    QVector3D getAcc() const;

    void setPos(QVector3D val);
    void setVel(QVector3D val);
    void setAcc(QVector3D val);

  signals:
    void posChanged();
    void velChanged();
    void accChanged();

  private:
    QVector3D pos, acc, vel;
};

class Spring : public QObject {
    Q_OBJECT
  public:
    explicit Spring(QObject* parent = nullptr);

    Q_PROPERTY(Point* start READ getStart WRITE setStart NOTIFY startChanged);
    Q_PROPERTY(Point* end READ getEnd WRITE setEnd NOTIFY endChanged);

    void setStart(Point* val);
    void setEnd(Point* val);
    void setLength(float val);
    Point* getStart() const;
    Point* getEnd() const;
  signals:
    void startChanged();
    void endChanged();

  private:
    Point* startPoint;
    Point* endPoint;
    float desiredLength;
};

class Simulation : public QObject {
    Q_OBJECT

  public:
    static constexpr int size = 15;
    explicit Simulation(QObject* parent = nullptr);

    Q_PROPERTY(QVector3D framePos READ getFramePos WRITE setFramePos NOTIFY framePosChanged);
    Q_PROPERTY(QVector3D frameRot READ getFrameRot WRITE setFrameRot NOTIFY frameRotChanged);
    Q_PROPERTY(QQmlListProperty<Point> points READ getPoints NOTIFY pointsChanged);
    Q_PROPERTY(QQmlListProperty<Spring> springs READ getSprings NOTIFY springsChanged);

    void setFramePos(QVector3D val);
    void setFrameRot(QVector3D val);

    QVector3D getFramePos() const;
    QVector3D getFrameRot() const;
    QQmlListProperty<Point> getPoints();
    QQmlListProperty<Spring> getSprings();

    const QList<Spring*>& borrowSprings() const;
  signals:
    void framePosChanged();
    void frameRotChanged();
    void pointsChanged();
    void springsChanged();

  private:
    QVector3D framePosition;
    QVector3D frameRotation;
    QList<Point*> pts;
    QList<Spring*> sprs;
};
