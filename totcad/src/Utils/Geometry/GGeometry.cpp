#include "Utils/Geometry/GGeometry.hpp"

#include <algorithm>
#include <cmath>

namespace totcad::geometry
{
namespace
{

constexpr double Pi = 3.14159265358979323846;

double toRadians(double degrees)
{
    return degrees * Pi / 180.0;
}
double toDegrees(double radians)
{
    return radians * 180.0 / Pi;
}

double normalizeAngle(double angle)
{
    angle = std::fmod(angle, 360.0);
    return angle < 0.0 ? angle + 360.0 : angle;
}

double counterClockwiseSweep(double from, double to)
{
    return normalizeAngle(to - from);
}

Point operator+(const Point &a, const Point &b)
{
    return {a.x + b.x, a.y + b.y};
}
Point operator-(const Point &a, const Point &b)
{
    return {a.x - b.x, a.y - b.y};
}
Point operator*(const Point &point, double factor)
{
    return {point.x * factor, point.y * factor};
}

double dot(const Point &a, const Point &b)
{
    return a.x * b.x + a.y * b.y;
}
double cross(const Point &a, const Point &b)
{
    return a.x * b.y - a.y * b.x;
}
double squaredLength(const Point &point)
{
    return dot(point, point);
}

bool samePoint(const Point &a, const Point &b, double epsilon = 1.0e-7)
{
    return distance(a, b) <= epsilon;
}

bool angleOnArc(double angle, const Arc &arc, double epsilon = 1.0e-7)
{
    if (std::abs(arc.sweepAngle) >= 360.0 - epsilon)
        return true;
    if (arc.sweepAngle >= 0.0)
        return counterClockwiseSweep(arc.startAngle, angle) <= arc.sweepAngle + epsilon;
    return counterClockwiseSweep(angle, arc.startAngle) <= -arc.sweepAngle + epsilon;
}

Point pointAtAngle(const Arc &arc, double angle)
{
    const double radians = toRadians(angle);
    return {arc.center.x + arc.radius * std::cos(radians), arc.center.y + arc.radius * std::sin(radians)};
}

void appendUnique(std::vector<Point> *points, const Point &point)
{
    if (!points)
        return;
    const auto duplicate =
        std::find_if(points->begin(), points->end(), [&](const Point &item) { return samePoint(item, point); });
    if (duplicate == points->end())
        points->push_back(point);
}

bool pointOnSegment(const Point &point, const Segment &segment)
{
    return distanceToSegment(point, segment) <= 1.0e-7;
}

} // namespace

double distance(const Point &first, const Point &second)
{
    return std::hypot(first.x - second.x, first.y - second.y);
}

double distanceToLine(const Point &point, const Segment &line)
{
    const Point direction = line.end - line.start;
    const double length = std::hypot(direction.x, direction.y);
    if (length <= Epsilon)
        return distance(point, line.start);
    return std::abs(cross(direction, point - line.start)) / length;
}

double distanceToSegment(const Point &point, const Segment &segment)
{
    const Point direction = segment.end - segment.start;
    const double lengthSquared = squaredLength(direction);
    if (lengthSquared <= Epsilon)
        return distance(point, segment.start);

    double parameter = dot(point - segment.start, direction) / lengthSquared;
    parameter = std::clamp(parameter, 0.0, 1.0);
    return distance(point, segment.start + direction * parameter);
}

double distanceToArc(const Point &point, const Arc &arc)
{
    if (arc.radius <= Epsilon)
        return distance(point, arc.center);

    const double angle = toDegrees(std::atan2(point.y - arc.center.y, point.x - arc.center.x));
    if (angleOnArc(angle, arc))
        return std::abs(distance(point, arc.center) - arc.radius);

    return std::min(distance(point, startPoint(arc)), distance(point, endPoint(arc)));
}

Rect boundingRect(const Segment &segment)
{
    return {std::min(segment.start.x, segment.end.x), std::min(segment.start.y, segment.end.y),
            std::max(segment.start.x, segment.end.x), std::max(segment.start.y, segment.end.y)};
}

Rect boundingRect(const Arc &arc)
{
    const Point first = startPoint(arc);
    const Point last = endPoint(arc);
    Rect result{std::min(first.x, last.x), std::min(first.y, last.y), std::max(first.x, last.x),
                std::max(first.y, last.y)};

    constexpr double cardinalAngles[]{0.0, 90.0, 180.0, 270.0};
    for (double angle : cardinalAngles)
    {
        if (!angleOnArc(angle, arc))
            continue;
        const Point point = pointAtAngle(arc, angle);
        result.minX = std::min(result.minX, point.x);
        result.minY = std::min(result.minY, point.y);
        result.maxX = std::max(result.maxX, point.x);
        result.maxY = std::max(result.maxY, point.y);
    }
    return result;
}

bool intersect(const Segment &first, const Segment &second, std::vector<Point> *points)
{
    if (points)
        points->clear();

    const Point r = first.end - first.start;
    const Point s = second.end - second.start;
    const double rLengthSquared = squaredLength(r);
    const double sLengthSquared = squaredLength(s);

    if (rLengthSquared <= Epsilon)
    {
        if (!pointOnSegment(first.start, second))
            return false;
        appendUnique(points, first.start);
        return true;
    }
    if (sLengthSquared <= Epsilon)
    {
        if (!pointOnSegment(second.start, first))
            return false;
        appendUnique(points, second.start);
        return true;
    }

    const Point difference = second.start - first.start;
    const double denominator = cross(r, s);
    if (std::abs(denominator) <= Epsilon)
    {
        if (std::abs(cross(difference, r)) > Epsilon)
            return false;

        const double t0 = dot(difference, r) / rLengthSquared;
        const double t1 = t0 + dot(s, r) / rLengthSquared;
        const double low = std::max(0.0, std::min(t0, t1));
        const double high = std::min(1.0, std::max(t0, t1));
        if (low > high + Epsilon)
            return false;

        appendUnique(points, first.start + r * low);
        if (high > low + Epsilon)
            appendUnique(points, first.start + r * high);
        return true;
    }

    const double t = cross(difference, s) / denominator;
    const double u = cross(difference, r) / denominator;
    if (t < -Epsilon || t > 1.0 + Epsilon || u < -Epsilon || u > 1.0 + Epsilon)
        return false;

    appendUnique(points, first.start + r * t);
    return true;
}

bool intersect(const Segment &segment, const Arc &arc, std::vector<Point> *points)
{
    if (points)
        points->clear();

    const Point direction = segment.end - segment.start;
    const Point relative = segment.start - arc.center;
    const double a = dot(direction, direction);
    if (a <= Epsilon)
    {
        if (!contains(arc, segment.start))
            return false;
        appendUnique(points, segment.start);
        return true;
    }

    const double b = 2.0 * dot(relative, direction);
    const double c = dot(relative, relative) - arc.radius * arc.radius;
    double discriminant = b * b - 4.0 * a * c;
    if (discriminant < -Epsilon)
        return false;

    discriminant = std::max(0.0, discriminant);
    const double root = std::sqrt(discriminant);
    const double parameters[]{(-b - root) / (2.0 * a), (-b + root) / (2.0 * a)};
    bool found = false;
    for (double parameter : parameters)
    {
        if (parameter < -Epsilon || parameter > 1.0 + Epsilon)
            continue;
        const Point candidate = segment.start + direction * parameter;
        if (contains(arc, candidate))
        {
            found = true;
            appendUnique(points, candidate);
        }
    }
    return found;
}

bool intersect(const Arc &first, const Arc &second, std::vector<Point> *points)
{
    if (points)
        points->clear();

    const double centerDistance = distance(first.center, second.center);
    if (centerDistance <= Epsilon && std::abs(first.radius - second.radius) <= Epsilon)
    {
        const Point candidates[]{startPoint(first), endPoint(first), startPoint(second), endPoint(second)};
        bool found = false;
        for (const Point &candidate : candidates)
        {
            if (contains(first, candidate) && contains(second, candidate))
            {
                found = true;
                appendUnique(points, candidate);
            }
        }
        return found;
    }

    if (centerDistance <= Epsilon || centerDistance > first.radius + second.radius + Epsilon ||
        centerDistance < std::abs(first.radius - second.radius) - Epsilon)
        return false;

    const double along =
        (first.radius * first.radius - second.radius * second.radius + centerDistance * centerDistance) /
        (2.0 * centerDistance);
    double heightSquared = first.radius * first.radius - along * along;
    if (heightSquared < -Epsilon)
        return false;
    const double height = std::sqrt(std::max(0.0, heightSquared));
    const Point unit = (second.center - first.center) * (1.0 / centerDistance);
    const Point base = first.center + unit * along;
    const Point normal{-unit.y, unit.x};
    const Point candidates[]{base + normal * height, base - normal * height};

    bool found = false;
    for (const Point &candidate : candidates)
    {
        if (contains(first, candidate) && contains(second, candidate))
        {
            found = true;
            appendUnique(points, candidate);
        }
    }
    return found;
}

bool arcFromThreePoints(const Point &start, const Point &middle, const Point &end, Arc *result)
{
    if (!result || samePoint(start, middle) || samePoint(middle, end) || samePoint(start, end))
        return false;

    const double determinant =
        2.0 * (start.x * (middle.y - end.y) + middle.x * (end.y - start.y) + end.x * (start.y - middle.y));
    if (std::abs(determinant) <= Epsilon)
        return false;

    const double startSquared = start.x * start.x + start.y * start.y;
    const double middleSquared = middle.x * middle.x + middle.y * middle.y;
    const double endSquared = end.x * end.x + end.y * end.y;

    Arc arc;
    arc.center.x =
        (startSquared * (middle.y - end.y) + middleSquared * (end.y - start.y) + endSquared * (start.y - middle.y)) /
        determinant;
    arc.center.y =
        (startSquared * (end.x - middle.x) + middleSquared * (start.x - end.x) + endSquared * (middle.x - start.x)) /
        determinant;
    arc.radius = distance(arc.center, start);

    const auto angleOf = [&](const Point &point) {
        return toDegrees(std::atan2(point.y - arc.center.y, point.x - arc.center.x));
    };
    const double startAngle = angleOf(start);
    const double middleAngle = angleOf(middle);
    const double endAngle = angleOf(end);

    arc.startAngle = normalizeAngle(startAngle);
    const double counterClockwise = counterClockwiseSweep(startAngle, endAngle);
    arc.sweepAngle = counterClockwiseSweep(startAngle, middleAngle) <= counterClockwise + 1.0e-7
                         ? counterClockwise
                         : -(360.0 - counterClockwise);

    *result = arc;
    return true;
}

Point startPoint(const Arc &arc)
{
    return pointAtAngle(arc, arc.startAngle);
}
Point endPoint(const Arc &arc)
{
    return pointAtAngle(arc, arc.startAngle + arc.sweepAngle);
}

bool contains(const Arc &arc, const Point &point, double epsilon)
{
    if (arc.radius < 0.0 || std::abs(distance(arc.center, point) - arc.radius) > epsilon)
        return false;
    const double angle = toDegrees(std::atan2(point.y - arc.center.y, point.x - arc.center.x));
    return angleOnArc(angle, arc, epsilon);
}

} // namespace totcad::geometry
