#pragma once
#include <QObject>
#include <QQuickImageProvider>
//#include <QImage>

class Simulation;

class CSpaceImageProvider : public QObject, public QQuickImageProvider {
    Q_OBJECT
  public:
    constexpr static int width  = 360;
    constexpr static int height = 360;

    explicit CSpaceImageProvider(QObject* parent = nullptr);

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

    void setCSpaceImage(const Simulation* cSpace);
    void setCSpaceImage(const Simulation* cSpace, const std::vector<std::pair<int, int>>& path);

    void setCSpaceImage(const Simulation* cSpace,
                        const std::vector<std::pair<int, int>>& path,
                        const std::array<std::array<int, 360>, 360>& distances_from_start);
    void resetCSpaceImage();

  signals:
    void imageChanged(const QString& id);

  private:
    QImage cSpaceImage;
};
