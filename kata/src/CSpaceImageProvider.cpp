#include "CSpaceImageProvider.hpp"

#include <cmath>

//#include "GeometryAlgorithms.hpp"
#include <iostream>
#include "simulation.hpp"

CSpaceImageProvider::CSpaceImageProvider(QObject* parent)
    : QObject(parent),
      QQuickImageProvider(QQuickImageProvider::Image),
      cSpaceImage(QSize(Simulation::a1Division, Simulation::a2Division), QImage::Format_RGB32) {
    resetCSpaceImage();
}
QImage CSpaceImageProvider::requestImage(const QString& id,
                                         QSize* size,
                                         const QSize& requestedSize) {
    std::cout << "got request for " << id.toStdString() << std::endl;
    if (id == "cSpace") {
        if (size) *size = QSize(cSpaceImage.width(), cSpaceImage.height());
        return cSpaceImage;
    } else {
        if (size) *size = QSize(0, 0);
        return QImage();
    }
}

void CSpaceImageProvider::setCSpaceImage(const Simulation* cSpace) {
    for (int a1 = 0; a1 < Simulation::a1Division; ++a1)
        for (int a2 = 0; a2 < Simulation::a2Division; ++a2) {
            cSpaceImage.setPixelColor(
                a1, a2, cSpace->colliding(a1, a2) ? QColor::fromRgb(229, 57, 53) : Qt::white);
        }
    emit imageChanged("cSpace");
}

void CSpaceImageProvider::setCSpaceImage(const Simulation* cSpace,
                                         const std::vector<std::pair<int, int>>& path) {
    for (int a1 = 0; a1 < Simulation::a1Division; ++a1)
        for (int a2 = 0; a2 < Simulation::a2Division; ++a2) {
            cSpaceImage.setPixelColor(
                a1, a2, cSpace->colliding(a1, a2) ? QColor::fromRgb(239, 154, 154) : Qt::white);
        }
    auto length = path.size();
    for (int i = 0; i < length; ++i) {
        const auto& point = path[i];
        auto factor       = (i / static_cast<float>(length));
        cSpaceImage.setPixelColor(
            point.first, point.second, QColor::fromRgb(56, static_cast<int>(142 * factor), 60));
    }
    emit imageChanged("cSpace");
}

void CSpaceImageProvider::setCSpaceImage(
    const Simulation* cSpace,
    const std::vector<std::pair<int, int>>& path,
    const std::array<std::array<int, 360>, 360>& distances_from_start) {
    int maxDist = 0;

    for (int a1 = 0; a1 < Simulation::a1Division; ++a1)
        for (int a2 = 0; a2 < Simulation::a2Division; ++a2)
            if (distances_from_start[a1][a2] < 10000 && distances_from_start[a1][a2] > maxDist)
                maxDist = distances_from_start[a1][a2];

    std::cout << "max dist: " << maxDist << std::endl;

    for (int a1 = 0; a1 < Simulation::a1Division; ++a1) {
        for (int a2 = 0; a2 < Simulation::a2Division; ++a2) {
            //cSpaceImage.setPixelColor(a1, a2, cSpace->colliding(a1, a2) ? QColor::fromRgb(239, 154, 154) : Qt::white);
            if (distances_from_start[a1][a2] > 10000)
                cSpaceImage.setPixelColor(a1, a2, QColor::fromRgb(239, 5, 15));
            else if (distances_from_start[a1][a2] == -1)
                cSpaceImage.setPixelColor(a1, a2, Qt::white);
            else {
                float factor = distances_from_start[a1][a2] / static_cast<float>(maxDist);
                cSpaceImage.setPixelColor(
                    a1,
                    a2,
                    QColor::fromRgb(
                        0, static_cast<int>(255 * (1 - factor)), static_cast<int>(255 * factor)));
            }
        }
    }

    for (int i = 0; i < path.size(); ++i) {
        const auto& point = path[i];
        cSpaceImage.setPixelColor(point.first, point.second, Qt::black);
    }

    emit imageChanged("cSpace");
}

void CSpaceImageProvider::resetCSpaceImage() {
    cSpaceImage.fill(Qt::black);
    emit imageChanged("cSpace");
}
