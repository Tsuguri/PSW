#pragma once

#include <vector>

#include <QObject>
#include <QTimer>

#include "CSpaceImageProvider.hpp"

class Box : public QObject {
    Q_OBJECT
  public:
    explicit Box(QObject* parent = nullptr);

    Q_PROPERTY(int x1 READ getX1 WRITE setX1 NOTIFY boxChanged);
    Q_PROPERTY(int y1 READ getY1 WRITE setY1 NOTIFY boxChanged);
    Q_PROPERTY(int x2 READ getX2 WRITE setX2 NOTIFY boxChanged);
    Q_PROPERTY(int y2 READ getY2 WRITE setY2 NOTIFY boxChanged);

    int getX1() const;
    int getY1() const;
    int getX2() const;
    int getY2() const;

    void setX1(int val);
    void setY1(int val);
    void setX2(int val);
    void setY2(int val);

  signals:
    void boxChanged();

  private:
    int px1, py1, px2, py2;
};

class Simulation : public QObject {
    Q_OBJECT

  public:
    constexpr static int a1Division = 360;
    constexpr static int a2Division = 360;
    explicit Simulation(QObject* parent = nullptr);

    Q_PROPERTY(bool showOptions READ getShowOptions NOTIFY showOptionsChanged);
    Q_PROPERTY(float r1 READ getR1 WRITE setR1 NOTIFY r1Changed);
    Q_PROPERTY(float r2 READ getR2 WRITE setR2 NOTIFY r2Changed);

    Q_PROPERTY(float startA1 READ getStartA1 NOTIFY startChanged);
    Q_PROPERTY(float startA2 READ getStartA2 NOTIFY startChanged);

    Q_PROPERTY(float endA1 READ getEndA1 NOTIFY endChanged);
    Q_PROPERTY(float endA2 READ getEndA2 NOTIFY endChanged);

    Q_PROPERTY(float a1 READ geta1 NOTIFY stateChanged);
    Q_PROPERTY(float a2 READ geta2 NOTIFY stateChanged);

    Q_PROPERTY(float a1a READ getA1A NOTIFY a1aChanged);
    Q_PROPERTY(float a2a READ getA2A NOTIFY a2aChanged);
    Q_PROPERTY(float a1b READ getA1B NOTIFY a1bChanged);
    Q_PROPERTY(float a2b READ getA2B NOTIFY a2bChanged);
    Q_PROPERTY(int rects READ getRects NOTIFY rectsChanged);

    Q_PROPERTY(QQmlListProperty<Box> rectangles READ rectangles NOTIFY rectsChanged);

    bool getShowOptions() const;
    int getRects() const;
    float getR1() const;
    float getR2() const;
    float getA1A() const;
    float getA2A() const;

    float getA1B() const;
    float getA2B() const;

    float geta1() const;
    float geta2() const;

    float getStartA1() const;
    float getStartA2() const;
    float getEndA1() const;
    float getEndA2() const;

    void setR1(float val);
    void setR2(float val);

    Q_INVOKABLE void startSimulation();
    Q_INVOKABLE void clearRects();

    Q_INVOKABLE void addRect(int x1, int y1, int x2, int y2);
    Q_INVOKABLE void removeRext(int index);

    Q_INVOKABLE void run(CSpaceImageProvider* provi);
    Q_INVOKABLE void stop();

    Q_INVOKABLE void confirmA();
    Q_INVOKABLE void confirmB();

    Q_INVOKABLE void placeStart(int x, int y);
    Q_INVOKABLE void placeEnd(int x, int y);

    bool colliding(int a1, int a2) const;

    QQmlListProperty<Box> rectangles();

  signals:
    void stateChanged();
    void showOptionsChanged();
    void r1Changed();
    void r2Changed();
    void rectsChanged();

    void a1aChanged();
    void a2aChanged();
    void a1bChanged();
    void a2bChanged();

    void startChanged();
    void endChanged();

  private:
    void tick();
    std::vector<std::pair<int, int>> BFS(int a1, int a2, int b1, int b2);
    std::tuple<float, float, float, float> InverseKinematics(int x, int y);
    bool a, b, showOpt;
    float r1, r2;
    std::vector<std::tuple<int, int, int, int>> rects;

    QList<Box*> obstacles;

    float a1, a2, startA1, startA2, endA1, endA2;
    float a1a, a2a, a2b, a1b;

    bool selectingA;

    std::vector<std::pair<int, int>> path;
    std::array<std::array<int, 360>, 360> available;
    QTimer* timer;
    int frame;
};
