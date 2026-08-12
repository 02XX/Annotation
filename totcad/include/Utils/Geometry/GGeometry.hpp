#pragma once

#include <vector>

namespace totcad::geometry
{

inline constexpr double Epsilon = 1.0e-9;

struct Point
{
    double x{0.0};
    double y{0.0};
};

struct Segment
{
    Point start;
    Point end;
};

// Angles are expressed in degrees. A positive sweep is counter-clockwise.
struct Arc
{
    Point center;
    double radius{0.0};
    double startAngle{0.0};
    double sweepAngle{0.0};
};

struct Rect
{
    double minX{0.0};
    double minY{0.0};
    double maxX{0.0};
    double maxY{0.0};
};

double distance(const Point &first, const Point &second);
double distanceToLine(const Point &point, const Segment &line);
double distanceToSegment(const Point &point, const Segment &segment);
double distanceToArc(const Point &point, const Arc &arc);

Rect boundingRect(const Segment &segment);
Rect boundingRect(const Arc &arc);

bool intersect(const Segment &first, const Segment &second, std::vector<Point> *intersectionPoints = nullptr);
bool intersect(const Segment &segment, const Arc &arc, std::vector<Point> *intersectionPoints = nullptr);
bool intersect(const Arc &first, const Arc &second, std::vector<Point> *intersectionPoints = nullptr);

// Creates the unique arc from start to end that passes through middle.
// Returns false for duplicate or collinear points.
bool arcFromThreePoints(const Point &start, const Point &middle, const Point &end, Arc *result);

Point startPoint(const Arc &arc);
Point endPoint(const Arc &arc);
bool contains(const Arc &arc, const Point &point, double epsilon = 1.0e-7);

} // namespace totcad::geometry
