#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <memory>

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

class Simulation : public QObject{
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged);
    Q_PROPERTY(bool paused READ getPaused NOTIFY pausedChanged);

    Q_PROPERTY(double position READ getPosition NOTIFY positionChanged);
    Q_PROPERTY(double velocity READ getVelocity NOTIFY velocityChanged);
    Q_PROPERTY(double acceleration READ getAcceleration NOTIFY accelerationChanged);

    Q_PROPERTY(double time READ getTime NOTIFY timeChanged);

    public:
        explicit Simulation(QObject* parent = nullptr);
        ~Simulation();

        bool getRunning() const;
        bool getPaused() const;
        double getPosition() const;
        double getVelocity() const;
        double getAcceleration() const;
        double getTime() const;


        void tick();

        Q_INVOKABLE void runSimulation(double dt, double mass);

        Q_INVOKABLE void togglePause();
        Q_INVOKABLE void reset();

signals:
        void runningChanged();
        void pausedChanged();

        void positionChanged();
        void velocityChanged();
        void accelerationChanged();
        void timeChanged();

    private:
        double dt;

        bool running;
        bool paused;
        double time;
        std::unique_ptr<QTimer> timer;
};

#endif // SIMULATOR_HPP
