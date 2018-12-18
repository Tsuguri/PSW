#pragma once

#include <vector>

#include<QObject>

class Simulation : public QObject {
    Q_OBJECT

    public:
        explicit Simulation(QObject* parent = nullptr);

        Q_PROPERTY(bool showOptions READ getShowOptions NOTIFY showOptionsChanged);
        Q_PROPERTY(float r1 READ getR1 WRITE setR1 NOTIFY r1Changed);
        Q_PROPERTY(float r2 READ getR2 WRITE setR2 NOTIFY r2Changed);

        Q_PROPERTY(float startA1  READ getStartA1 NOTIFY startChanged);
        Q_PROPERTY(float startA2  READ getStartA2 NOTIFY startChanged);

        Q_PROPERTY(float endA1  READ getEndA1 NOTIFY endChanged);
        Q_PROPERTY(float endA2  READ getEndA2 NOTIFY endChanged);

        Q_PROPERTY(float a1a  READ getA1A NOTIFY a1aChanged);
        Q_PROPERTY(float a2a  READ getA2A NOTIFY a2aChanged);
        Q_PROPERTY(float a1b  READ getA1B NOTIFY a1bChanged);
        Q_PROPERTY(float a2b  READ getA2B NOTIFY a2bChanged);
        Q_PROPERTY(int rects READ getRects NOTIFY rectsChanged);


        bool getShowOptions() const;
        int getRects() const;
        float getR1() const;
        float getR2() const;
        float getA1A() const;
        float getA2A() const;
        float getA1B() const;
        float getA2B() const;

        float getStartA1() const;
        float getStartA2() const;
        float getEndA1() const;
        float getEndA2() const;


        void setR1(float val);
        void setR2(float val);

        Q_INVOKABLE void startSimulation();
        Q_INVOKABLE void clearRects();
        Q_INVOKABLE void confirmA();
        Q_INVOKABLE void confirmB();

        Q_INVOKABLE void placeStart(int x, int y);
        Q_INVOKABLE void placeEnd(int x, int y);

signals:
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
std::tuple<float, float, float, float> InverseKinematics(int x, int y);
        bool a,b, showOpt;
        float r1,r2;
        std::vector<int> rects;

        float a1,a2, startA1, startA2, endA1, endA2;
        float a1a, a2a, a2b, a1b;

        bool selectingA;

};
