#include "simulation.hpp"
#include <cmath>
#include <iostream>

#include <list>

Box::Box(QObject* parent) : QObject(parent) {
}

int Box::getX1() const {
    return px1;
}

int Box::getY1() const {
    return py1;
}

int Box::getX2() const {
    return px2;
}

int Box::getY2() const {
    return py2;
}

void Box::setX1(int val) {
    px1 = val;
    emit boxChanged();
}
void Box::setY1(int val) {
    py1 = val;
    emit boxChanged();
}
void Box::setX2(int val) {
    px2 = val;
    emit boxChanged();
}
void Box::setY2(int val) {
    py2 = val;
    emit boxChanged();
}

Simulation::Simulation(QObject* parent)
    : QObject(parent), a(false), b(false), showOpt(false), timer(new QTimer(this)), available() {
    timer->setInterval(20);
    QObject::connect(timer, &QTimer::timeout, this, &Simulation::tick);
    startA1=50;
    endA1=startA1;
    startA2=40;
    endA2=40;
    emit startChanged();
    emit endChanged();
}

bool Simulation::getShowOptions() const {
    return showOpt;
}

int Simulation::getRects() const {
    return rects.size();
}

void Simulation::setR1(float val) {
    r1 = val;
    emit r1Changed();
}

void Simulation::setR2(float val) {
    r2 = val;
    emit r2Changed();
}

float Simulation::getR1() const {
    return r1;
}

float Simulation::getR2() const {
    return r2;
}
float Simulation::getStartA1() const {
    return startA1;
}

float Simulation::getStartA2() const {
    return startA2;
}

float Simulation::getEndA1() const {
    return endA1;
}

float Simulation::getEndA2() const {
    return endA2;
}

void Simulation::startSimulation() {
    std::cout << "start simulation<<" << std::endl;
}

void Simulation::clearRects() {
    rects.clear();
    emit rectsChanged();
}

void Simulation::addRect(int x1, int y1, int x2, int y2) {
    rects.emplace_back(x1, y2, x2, y2);
    auto box = new Box(this);
    box->setX1(std::min(x1, x2));
    box->setY1(std::min(y1, y2));
    box->setX2(std::max(x1, x2));
    box->setY2(std::max(y1, y2));
    obstacles.push_back(box);

    emit rectsChanged();
}

void Simulation::removeRext(int index) {
    obstacles.removeAt(index);
    emit rectsChanged();
}

void Simulation::confirmA() {
    if (selectingA) {
        startA1 = a1a;
        startA2 = a2a;
        emit startChanged();
    } else {
        endA1 = a1a;
        endA2 = a2a;
        emit endChanged();
    }

    showOpt = false;
    emit showOptionsChanged();
    // emit change of start/end
}

void Simulation::confirmB() {
    if (selectingA) {
        startA1 = a1b;
        startA2 = a2b;
        emit startChanged();
    } else {
        endA1 = a1b;
        endA2 = a2b;
        emit endChanged();
    }

    showOpt = false;
    emit showOptionsChanged();
}

static constexpr float Pi = 3.14;
std::tuple<float, float, float, float> Simulation::InverseKinematics(int x, int y) {
    auto dist     = [](int a, int b) { return std::sqrt(a * a + b * b); };
    float length0 = r1;
    float length1 = r2;
    float length2 = dist(x, y);
    std::cout << "l1: " << r1 << " l2: " << r2 << "l3:" << length2 << std::endl;

    float cosAngle0 =
        ((length0 * length0) + (length2 * length2) - (length1 * length1)) / (2 * length0 * length2);
    float angle0 = std::acos(cosAngle0) * 180 / Pi;
    std::cout << "cos angle0: " << cosAngle0 << " angle: " << angle0 << std::endl;

    // Inner angle beta
    float cosAngle1 =
        ((length1 * length1) + (length0 * length0) - (length2 * length2)) / (2 * length1 * length0);
    float angle1 = std::acos(cosAngle1) * 180 / Pi;

    // Angle from Joint0 and Target
    float atan = std::atan2(y, x) * 180 / Pi;
    std::cout << "atan: " << atan << std::endl;

    // So they work in Unity reference frame
    float jointAngle0 = (atan - angle0);  // Angle A
    float jointAngle1 = (180 - angle1);   // Angle B

    float jointAngle0B = (atan + angle0);
    float jointAngle1B = (180 + angle1);

    return std::make_tuple(jointAngle0, jointAngle1, jointAngle0B, jointAngle1B);
}

void Simulation::placeStart(int x, int y) {
    selectingA = true;

    auto angles = InverseKinematics(x, y);
    a1a         = std::get<0>(angles);
    a2a         = std::get<1>(angles);
    a1b         = std::get<2>(angles);
    a2b         = std::get<3>(angles);
    std::cout << "angles: " << a1a << " " << a2a << " " << a1b << " " << a2b << std::endl;
    // fill positions
    showOpt = true;
    emit showOptionsChanged();
    emit a1aChanged();
    emit a2aChanged();
    emit a1bChanged();
    emit a2bChanged();
}

void Simulation::placeEnd(int x, int y) {
    selectingA = false;

    auto angles = InverseKinematics(x, y);
    a1a         = std::get<0>(angles);
    a2a         = std::get<1>(angles);
    a1b         = std::get<2>(angles);
    a2b         = std::get<3>(angles);
    showOpt     = true;
    emit showOptionsChanged();
    emit a1aChanged();
    emit a2aChanged();
    emit a1bChanged();
    emit a2bChanged();
}

float Simulation::getA1A() const {
    return a1a;
}

float Simulation::getA2A() const {
    return a2a;
}

float Simulation::getA1B() const {
    return a1b;
}

float Simulation::getA2B() const {
    return a2b;
}

float Simulation::geta1() const {
    return a1;
}

float Simulation::geta2() const {
    return a2;
}

struct Point {
    double x;
    double y;
};
constexpr double eps = 0.00001;

double SideOfPoint(const Point& start, const Point& end, const Point& point) {
    return start.x * end.y + end.x * point.y + point.x * start.y - point.x * end.y -
           start.x * point.y - end.x * start.y;
}

bool Collinear(const Point& p1, const Point& p2, const Point& p3) {
    return std::abs(SideOfPoint(p1, p2, p3)) < eps;
}

bool PointOnSegment(const Point& start, const Point& end, const Point& point) {
    return std::min(start.x, end.x) - eps <= point.x && point.x <= std::max(start.x, end.x) + eps &&
           std::min(start.y, end.y) - eps <= point.y && point.y <= std::max(start.y, end.y) + eps &&
           Collinear(start, end, point);
}

bool segmentsCollide(const Point& s1, const Point& e1, const Point& s2, const Point& e2) {
    if (PointOnSegment(s1, e1, s2) || PointOnSegment(s1, e1, e2) || PointOnSegment(s2, e2, s1) ||
        PointOnSegment(s2, e2, e1))
        return true;

    if (SideOfPoint(s1, e1, s2) * SideOfPoint(s1, e1, e2) >= 0) return false;

    return SideOfPoint(s2, e2, s1) * SideOfPoint(s2, e2, e1) < 0;
}

bool isSegmentCollidingWithBox(const Point& p1, const Point& p2, Box* box) {
    auto& b = *box;
    if ((p1.x < b.getX1() && p2.x < b.getX1()) || (p1.x > b.getX2() && p2.x > b.getX2()) ||
        (p1.y < b.getY1() && p2.y < b.getY1()) || (p1.y > b.getY2() && p2.y > b.getY2())) {
        return false;
    }

    auto bxm   = b.getX1();
    auto bxmax = b.getX2();
    auto bym   = b.getY1();
    auto bymax = b.getY2();

    if ((p1.x > bxm && p1.x < bxmax && p1.y > bym && p1.y < bymax) &&
        (p2.x > bxm && p2.x < bxmax && p2.y > bym && p2.y < bymax)) {
        return true;
    }

    Point b1 = {bxm, bym};
    Point b2 = {bxm, bymax};
    Point b3 = {bxmax, bymax};
    Point b4 = {bxmax, bym};

    if (segmentsCollide(p1, p2, b1, b2) || segmentsCollide(p1, p2, b2, b3) ||
        segmentsCollide(p1, p2, b3, b4) || segmentsCollide(p1, p2, b4, b1)) {
        return true;
    }

    return false;
}

bool Simulation::colliding(int a1, int a2) const {
    bool ret = false;

    Point p0 = {0, 0};

    // uzupełnić o prawdziwe dane
    auto rad = [](auto x) { return x * M_PI / 180.0; };
    Point p1 = {std::cos(rad(a1)) * getR1(), std::sin(rad(a1)) * getR1()};

    Point p2 = {p1.x + std::cos(rad(a1 + a2)) * getR2(), p1.y + std::sin(rad(a1 + a2)) * getR2()};

    for (const auto& elem : obstacles) {
        if (isSegmentCollidingWithBox(p0, p1, elem) || isSegmentCollidingWithBox(p1, p2, elem)) {
            return true;
        }
    }
    return false;
}

void Simulation::run(CSpaceImageProvider* provi) {
    timer->start();
    frame = 0;

    path = BFS(startA1, startA2, endA1, endA2);
    provi->setCSpaceImage(this, path, available);
}

std::vector<std::pair<int, int>> Simulation::BFS(int a1, int a2, int b1, int b2) {
    std::list<std::tuple<int, int, int>> kolejka;

    a1 = (a1 + 360) % 360;
    a2 = (a2 + 360) % 360;
    b1 = (b1 + 360) % 360;
    b2 = (b2 + 360) % 360;
    std::cout << "path from " << a1 << " " << a2 << " to " << b1 << " " << b2 << std::endl;

    int n = 360;
    std::array<std::array<std::pair<int, int>, 360>, 360> visited;
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; i++)
            available[i][j] = colliding(i, j) ? 12345600 : -1;  // Wierzchołki nie odwiedzone

    kolejka.push_back(std::make_tuple(a1, a2, 0));
    visited[a1][a2]   = std::make_pair(-1, -1);
    available[a1][a2] = 0;

    if (a1 == b1 && a2 == b2) {
        auto x = std::get<0>(kolejka.front());
        auto y = std::get<1>(kolejka.front());
        return std::vector<std::pair<int, int>>{{x, y}};
    }

    int p = 1;
    while (!kolejka.empty()) {
        p++;
        // std::cout<<" in"<<std::endl;

        auto s = kolejka.front();
        kolejka.pop_front();  // Usuwamy odwiedzany element

        auto pt   = s;
        auto p1   = std::get<0>(pt);
        auto p2   = std::get<1>(pt);
        auto pts  = std::make_pair(p1, p2);
        auto dist = std::get<2>(pt);
        // std::cout<<" checking pt "<<p1<<" "<<p2<<std::endl;

        if (p1 == b1 && p2 == b2) {
            std::vector<std::pair<int, int>> result;
            // std::cout<<"result found"<<std::endl;

            auto c1 = p1;
            auto c2 = p2;
            while (c1 != -1 && c2 != -1) {
                // if( result.size()>10) {
                // throw 0;
                //}
                // std::cout<<"p: "<<c1<<" "<<c2<<std::endl;

                result.push_back(std::make_pair(c1, c2));
                auto pt = visited[c1][c2];
                c1      = std::get<0>(pt);
                c2      = std::get<1>(pt);
            }
            return result;
        }

        for (int j = -1; j <= 1; j += 2) {
            auto dx = p1;
            auto dy = (p2 + j + 360) % 360;
            if (available[dx][dy] == -1) {
                visited[dx][dy]   = pts;
                available[dx][dy] = dist + 1;
                kolejka.push_back(std::make_tuple(dx, dy, dist + 1));
                // std::cout<<"p: "<<dx<<" "<<dy<<" from: "<<p1<<" "<<p2<<std::endl;
            }

            dx = (p1 + j + 360) % 360;
            dy = p2;
            if (available[dx][dy] == -1) {
                visited[dx][dy]   = pts;
                available[dx][dy] = dist + 1;
                kolejka.push_back(std::make_tuple(dx, dy, dist + 1));
            }

            // std::cout<<"iksde"<<std::endl;
        }
        for (int j = -1; j <= 1; j += 2)
            for (int i = -1; i <= 1; i += 2) {
                auto dx = (p1 + j + 360) % 360;
                auto dy = (p2 + i + 360) % 360;
                if (available[dx][dy] == -1) {
                    visited[dx][dy]   = pts;
                    available[dx][dy] = dist + 1;
                    kolejka.emplace_back(dx, dy, dist + 1);
                }
            }
    }
    std::cout << "kolejka: " << kolejka.size() << std::endl;
    return std::vector<std::pair<int, int>>{std::make_pair(a1, a2)};
}

void Simulation::stop() {
    std::cout << "stopping" << std::endl;

    timer->stop();
    frame = -1;
}

void Simulation::tick() {
    frame++;
    if (frame >= path.size()) {
        frame = 0;
    }

    a1 = std::get<0>(path[frame]);
    a2 = std::get<1>(path[frame]);

    emit stateChanged();
}

QQmlListProperty<Box> Simulation::rectangles() {
    return QQmlListProperty<Box>(this, obstacles);
}
