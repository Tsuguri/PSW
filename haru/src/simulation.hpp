#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <memory>

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>

#include "math.hpp"

struct State {
  double position;
  double velocity;
  double time;
};

class Simulation : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool running READ getRunning NOTIFY runningChanged);
  Q_PROPERTY(bool paused READ getPaused NOTIFY pausedChanged);

  Q_PROPERTY(double position READ getPosition NOTIFY positionChanged);
  Q_PROPERTY(double velocity READ getVelocity NOTIFY velocityChanged);
  Q_PROPERTY(
      double acceleration READ getAcceleration NOTIFY accelerationChanged);

  Q_PROPERTY(double flexForce READ getFlexForce NOTIFY flexForceChanged);
  Q_PROPERTY(
      double dampingForce READ getDampeningForce NOTIFY dampeningForceChanged);
  Q_PROPERTY(
      double externalForce READ getExternalForce NOTIFY externalForceChanged);

  Q_PROPERTY(double time READ getTime NOTIFY timeChanged);
  Q_PROPERTY(double mass READ getMass NOTIFY massChanged);

  Q_PROPERTY(double originPosition READ getOriginPosition NOTIFY originChanged);

public:
  explicit Simulation(QObject *parent = nullptr);
  ~Simulation();

  bool getRunning() const;
  bool getPaused() const;
  double getPosition() const;
  double getVelocity() const;
  double getAcceleration() const;
  double getTime() const;

  double getFlexForce() const;
  double getDampeningForce() const;
  double getExternalForce() const;

  double getForce() const;

  double getMass() const;
  double getOriginPosition() const;

  void tick();

  Q_INVOKABLE void runSimulation(double dt, double mass, double p0, double v0,
                                 double damping, double flx, MathFormula* externalForce, MathFormula* originMovement);

  Q_INVOKABLE void togglePause();
  Q_INVOKABLE void reset();

signals:
  void runningChanged();
  void pausedChanged();

  void positionChanged();
  void velocityChanged();
  void accelerationChanged();

  void flexForceChanged();
  void dampeningForceChanged();
  void externalForceChanged();

  void timeChanged();
  void stepMade();
  void massChanged();
  void originChanged();

private:
  void StateChanged();
  double dt;
  double flex;
  double damping;
  double mass;

  bool running;
  bool paused;
  double time;
  State current;
  State previous;

  std::unique_ptr<QTimer> timer;
  std::unique_ptr<QElapsedTimer> elapsed;
  quint64 elapsedNotUsed;
  MathFormula* externalForce;
  MathFormula* originMovement;
};

#endif // SIMULATOR_HPP
