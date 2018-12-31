#pragma once
#include <QObject>
#include <QQmlListProperty>
#include <QTimer>
#include <QVector3D>

class Point : public QObject {
    Q_OBJECT
  public:
    explicit Point(QObject* parent = nullptr);

    Q_PROPERTY(QVector3D position READ getPos WRITE setPos NOTIFY posChanged);
    Q_PROPERTY(QVector3D velocity READ getVel WRITE setVel NOTIFY velChanged);
    //Q_PROPERTY(QVector3D acceleration READ getAcc WRITE setAcc NOTIFY accChanged);

    QVector3D getPos() const;
    QVector3D getVel() const;
    QVector3D getAcc() const;
    unsigned int getId() const;

    void setPos(QVector3D val);
    void setVel(QVector3D val);
    void setAcc(QVector3D val);
    void addAcc(QVector3D val);
    void setId(unsigned int val);

  signals:
    void posChanged();
    void velChanged();
    //void accChanged();

  private:
    QVector3D pos, acc, vel;
    unsigned int id;
};

class Spring : public QObject {
    Q_OBJECT
  public:
    explicit Spring(QObject* parent = nullptr, bool special = false);

    Q_PROPERTY(Point* start READ getStart WRITE setStart NOTIFY startChanged);
    Q_PROPERTY(Point* end READ getEnd WRITE setEnd NOTIFY endChanged);

    void setStart(Point* val);
    void setEnd(Point* val);
    void setLength(float val);
    Point* getStart() const;
    Point* getEnd() const;
    float getLength() const;
    bool isSpecial() const;
  signals:
    void startChanged();
    void endChanged();

  private:
    Point* startPoint;
    Point* endPoint;
    float desiredLength;
    bool special;
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

    Q_PROPERTY(float internalSpring READ getInternalSpring WRITE setInternalSpring NOTIFY
                   internalSpringChanged);
    Q_PROPERTY(float externalSpring READ getExternalSpring WRITE setExternalSpring NOTIFY
                   externalSpringChanged);

    Q_PROPERTY(float viscose READ getViscose WRITE setViscose NOTIFY viscoseChanged);
    Q_PROPERTY(float pointMass READ getMass WRITE setMass NOTIFY massChanged);

    Q_PROPERTY(float wallBouncing READ getBounce WRITE setBounce NOTIFY bounceChanged);

    void setFramePos(QVector3D val);
    void setFrameRot(QVector3D val);
    void setInternalSpring(float val);
    void setExternalSpring(float val);
    void setMass(float val);
    void setViscose(float val);
    void setBounce(float val);

    QVector3D getFramePos() const;
    QVector3D getFrameRot() const;
    QQmlListProperty<Point> getPoints();
    QQmlListProperty<Spring> getSprings();

    float getInternalSpring() const;
    float getExternalSpring() const;
    float getMass() const;
    float getViscose() const;
    float getBounce() const;

    const QList<Spring*>& borrowSprings() const;

    Q_INVOKABLE void randomizePositions();
  signals:
    void framePosChanged();
    void frameRotChanged();
    void pointsChanged();
    void springsChanged();
    void internalSpringChanged();
    void externalSpringChanged();
    void massChanged();
    void viscoseChanged();
    void bounceChanged();

  private:
    void tick();
    void computeCornerPoints();
    void collide();

    QVector3D framePosition;
    QVector3D frameRotation;
    QList<Point*> pts;

    float mass              = 0.2;
    float springness        = 2.0;
    float specialSpringness = 5.0;
    float viscosity         = 0.5;
    float bounce            = 0.1;

    std::array<Point*, 8> cubePts;
    QList<Spring*> sprs;
    QTimer* timer;
};
