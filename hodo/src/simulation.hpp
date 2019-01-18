#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <memory>
#include <random>

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>

struct State {
    double position;
    double velocity;
    double acceleration;
};

class Simulation : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged);
    Q_PROPERTY(bool paused READ getPaused NOTIFY pausedChanged);

    Q_PROPERTY(double w READ getW WRITE setW NOTIFY wChanged);
    Q_PROPERTY(double l READ getL WRITE setL NOTIFY lChanged);
    Q_PROPERTY(double r READ getR WRITE setR NOTIFY rChanged);
    Q_PROPERTY(double err READ getErr WRITE setErr NOTIFY errChanged);

    Q_PROPERTY(double angle READ getAngle NOTIFY stateChanged);
    Q_PROPERTY(double position READ getPosition NOTIFY stateChanged);
    Q_PROPERTY(double velocity READ getVelocity NOTIFY stateChanged);
    Q_PROPERTY(double acceleration READ getAcceleration NOTIFY stateChanged);

    Q_PROPERTY(double time READ getTime NOTIFY timeChanged);

  public:
    explicit Simulation(QObject* parent = nullptr);
    ~Simulation();

    void setW(double val);
    void setR(double val);
    void setL(double val);
    void setErr(double val);

    bool getRunning() const;
    bool getPaused() const;
    double getPosition() const;
    double getVelocity() const;
    double getAcceleration() const;
    double getTime() const;
    double getAngle() const;
    double getW() const;
    double getR() const;
    double getL() const;
    double getErr() const;

    void tick();

    Q_INVOKABLE void runSimulation(double dt);

    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void reset();

  signals:
    void runningChanged();
    void pausedChanged();

    void stateChanged();

    void timeChanged();
    void stepMade();

    void wChanged();
    void rChanged();
    void lChanged();
    void errChanged();

  private:
    void StateChanged();
    double dt;

    bool running;
    bool paused;
    double time;
    double w, r, l, err;
    double currentAngle;
    State current;
    State previous;

    std::unique_ptr<QTimer> timer;
    std::unique_ptr<QElapsedTimer> elapsed;
    quint64 elapsedNotUsed;
    std::random_device rd;
    std::mt19937 gen;
};

#endif  // SIMULATOR_HPP
