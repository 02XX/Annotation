#include "Utils/Geometry/GGeometry.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <vector>

using namespace totcad::geometry;

namespace
{

constexpr double Tolerance = 1.0e-7;

void expectPointNear(const Point &actual, const Point &expected)
{
    EXPECT_NEAR(actual.x, expected.x, Tolerance);
    EXPECT_NEAR(actual.y, expected.y, Tolerance);
}

void expectRectNear(const Rect &actual, const Rect &expected)
{
    EXPECT_NEAR(actual.minX, expected.minX, Tolerance);
    EXPECT_NEAR(actual.minY, expected.minY, Tolerance);
    EXPECT_NEAR(actual.maxX, expected.maxX, Tolerance);
    EXPECT_NEAR(actual.maxY, expected.maxY, Tolerance);
}

struct PointDistanceCase
{
    Point first;
    Point second;
    double expected;
};
class PointDistanceBatch : public testing::TestWithParam<PointDistanceCase>
{
};
TEST_P(PointDistanceBatch, CalculatesExpectedDistance)
{
    const auto &item = GetParam();
    EXPECT_NEAR(distance(item.first, item.second), item.expected, Tolerance);
    EXPECT_NEAR(distance(item.second, item.first), item.expected, Tolerance);
}
INSTANTIATE_TEST_SUITE_P(AllQuadrants, PointDistanceBatch,
                         testing::Values(PointDistanceCase{{0, 0}, {0, 0}, 0}, PointDistanceCase{{0, 0}, {3, 4}, 5},
                                         PointDistanceCase{{-2, -3}, {1, 1}, 5}, PointDistanceCase{{5, 2}, {-7, 2}, 12},
                                         PointDistanceCase{{3, -8}, {3, 4}, 12},
                                         PointDistanceCase{{0.5, 0.5}, {1.5, 1.5}, std::sqrt(2.0)}));

struct PointLineCase
{
    Point point;
    Segment line;
    double expected;
};
class PointLineBatch : public testing::TestWithParam<PointLineCase>
{
};
TEST_P(PointLineBatch, CalculatesInfiniteLineDistance)
{
    const auto &item = GetParam();
    EXPECT_NEAR(distanceToLine(item.point, item.line), item.expected, Tolerance);
}
INSTANTIATE_TEST_SUITE_P(HorizontalVerticalDiagonalAndDegenerate, PointLineBatch,
                         testing::Values(PointLineCase{{2, 3}, {{0, 0}, {4, 0}}, 3},
                                         PointLineCase{{3, 2}, {{0, 0}, {0, 5}}, 3},
                                         PointLineCase{{0, 2}, {{0, 0}, {2, 2}}, std::sqrt(2.0)},
                                         PointLineCase{{3, 4}, {{0, 0}, {0, 0}}, 5},
                                         PointLineCase{{2, 2}, {{-5, 2}, {9, 2}}, 0},
                                         PointLineCase{{-2, -1}, {{-3, -3}, {1, 1}}, std::sqrt(0.5)}));

struct PointSegmentCase
{
    Point point;
    Segment segment;
    double expected;
};
class PointSegmentBatch : public testing::TestWithParam<PointSegmentCase>
{
};
TEST_P(PointSegmentBatch, CalculatesClampedDistance)
{
    const auto &item = GetParam();
    EXPECT_NEAR(distanceToSegment(item.point, item.segment), item.expected, Tolerance);
}
INSTANTIATE_TEST_SUITE_P(InteriorEndpointsAndDegenerate, PointSegmentBatch,
                         testing::Values(PointSegmentCase{{2, 3}, {{0, 0}, {4, 0}}, 3},
                                         PointSegmentCase{{8, 3}, {{0, 0}, {4, 0}}, 5},
                                         PointSegmentCase{{-3, 4}, {{0, 0}, {4, 0}}, 5},
                                         PointSegmentCase{{0, 0}, {{0, 0}, {4, 0}}, 0},
                                         PointSegmentCase{{3, 4}, {{0, 0}, {0, 0}}, 5},
                                         PointSegmentCase{{1, 0}, {{0, 0}, {2, 2}}, std::sqrt(0.5)}));

struct ArcDistanceCase
{
    Point point;
    Arc arc;
    double expected;
};
class ArcDistanceBatch : public testing::TestWithParam<ArcDistanceCase>
{
};
TEST_P(ArcDistanceBatch, CalculatesFiniteArcDistance)
{
    const auto &item = GetParam();
    EXPECT_NEAR(distanceToArc(item.point, item.arc), item.expected, Tolerance);
}
INSTANTIATE_TEST_SUITE_P(RadialEndpointClockwiseAndDegenerate, ArcDistanceBatch,
                         testing::Values(ArcDistanceCase{{3, 0}, {{0, 0}, 5, 0, 90}, 2},
                                         ArcDistanceCase{{10, 0}, {{0, 0}, 5, 0, 90}, 5},
                                         ArcDistanceCase{{-5, 0}, {{0, 0}, 5, 0, 90}, std::sqrt(50.0)},
                                         ArcDistanceCase{{0, -3}, {{0, 0}, 5, 0, -90}, 2},
                                         ArcDistanceCase{{4, 6}, {{1, 2}, 0, 0, 90}, 5},
                                         ArcDistanceCase{{0, 0}, {{0, 0}, 5, 0, 360}, 5}));

struct SegmentRectCase
{
    Segment segment;
    Rect expected;
};
class SegmentRectBatch : public testing::TestWithParam<SegmentRectCase>
{
};
TEST_P(SegmentRectBatch, CalculatesAxisAlignedBounds)
{
    const auto &item = GetParam();
    expectRectNear(boundingRect(item.segment), item.expected);
}
INSTANTIATE_TEST_SUITE_P(AllDirections, SegmentRectBatch,
                         testing::Values(SegmentRectCase{{{4, -2}, {-1, 3}}, {-1, -2, 4, 3}},
                                         SegmentRectCase{{{0, 0}, {5, 0}}, {0, 0, 5, 0}},
                                         SegmentRectCase{{{2, 7}, {2, -1}}, {2, -1, 2, 7}},
                                         SegmentRectCase{{{-3, -4}, {-3, -4}}, {-3, -4, -3, -4}}));

struct ArcRectCase
{
    Arc arc;
    Rect expected;
};
class ArcRectBatch : public testing::TestWithParam<ArcRectCase>
{
};
TEST_P(ArcRectBatch, CalculatesExactBounds)
{
    const auto &item = GetParam();
    expectRectNear(boundingRect(item.arc), item.expected);
}
INSTANTIATE_TEST_SUITE_P(
    CardinalFullWrappedAndClockwise, ArcRectBatch,
    testing::Values(ArcRectCase{{{0, 0}, 2, 0, 90}, {0, 0, 2, 2}}, ArcRectCase{{{0, 0}, 2, 90, 90}, {-2, 0, 0, 2}},
                    ArcRectCase{{{0, 0}, 2, 180, 90}, {-2, -2, 0, 0}}, ArcRectCase{{{0, 0}, 2, 270, 90}, {0, -2, 2, 0}},
                    ArcRectCase{{{1, 2}, 3, 0, 360}, {-2, -1, 4, 5}}, ArcRectCase{{{0, 0}, 2, 0, -90}, {0, -2, 2, 0}},
                    ArcRectCase{{{1, 2}, 3, 300, 120},
                                {2.5, 2 - 3 * std::sqrt(3.0) / 2, 4, 2 + 3 * std::sqrt(3.0) / 2}}));

struct ContainsCase
{
    Arc arc;
    Point point;
    bool expected;
};
class ArcContainsBatch : public testing::TestWithParam<ContainsCase>
{
};
TEST_P(ArcContainsBatch, DetectsPointOnFiniteArc)
{
    const auto &item = GetParam();
    EXPECT_EQ(contains(item.arc, item.point), item.expected);
}
INSTANTIATE_TEST_SUITE_P(
    EndpointsInteriorSweepAndRadius, ArcContainsBatch,
    testing::Values(ContainsCase{{{0, 0}, 5, 0, 90}, {5, 0}, true}, ContainsCase{{{0, 0}, 5, 0, 90}, {0, 5}, true},
                    ContainsCase{{{0, 0}, 5, 0, 90}, {5 / std::sqrt(2.0), 5 / std::sqrt(2.0)}, true},
                    ContainsCase{{{0, 0}, 5, 0, 90}, {-5, 0}, false}, ContainsCase{{{0, 0}, 5, 0, 90}, {4, 0}, false},
                    ContainsCase{{{0, 0}, 5, 0, -90}, {0, -5}, true}, ContainsCase{{{0, 0}, 5, 300, 120}, {5, 0}, true},
                    ContainsCase{{{0, 0}, -1, 0, 90}, {-1, 0}, false}));

struct SegmentIntersectionCase
{
    Segment first;
    Segment second;
    bool expected;
    std::size_t pointCount;
};
class SegmentIntersectionBatch : public testing::TestWithParam<SegmentIntersectionCase>
{
};
TEST_P(SegmentIntersectionBatch, DetectsIntersectionAndCount)
{
    const auto &item = GetParam();
    std::vector<Point> points;
    EXPECT_EQ(intersect(item.first, item.second, &points), item.expected);
    EXPECT_EQ(points.size(), item.pointCount);
    EXPECT_EQ(intersect(item.first, item.second, nullptr), item.expected);
}
INSTANTIATE_TEST_SUITE_P(CrossTouchParallelOverlapAndPoints, SegmentIntersectionBatch,
                         testing::Values(SegmentIntersectionCase{{{0, 0}, {4, 4}}, {{0, 4}, {4, 0}}, true, 1},
                                         SegmentIntersectionCase{{{0, 0}, {4, 0}}, {{4, 0}, {8, 0}}, true, 1},
                                         SegmentIntersectionCase{{{0, 0}, {5, 0}}, {{2, 0}, {7, 0}}, true, 2},
                                         SegmentIntersectionCase{{{0, 0}, {1, 0}}, {{2, 0}, {3, 0}}, false, 0},
                                         SegmentIntersectionCase{{{0, 0}, {4, 0}}, {{0, 1}, {4, 1}}, false, 0},
                                         SegmentIntersectionCase{{{2, 0}, {2, 5}}, {{0, 3}, {4, 3}}, true, 1},
                                         SegmentIntersectionCase{{{1, 1}, {1, 1}}, {{0, 1}, {2, 1}}, true, 1},
                                         SegmentIntersectionCase{{{3, 1}, {3, 1}}, {{0, 1}, {2, 1}}, false, 0}));

struct SegmentArcIntersectionCase
{
    Segment segment;
    Arc arc;
    bool expected;
    std::size_t pointCount;
};
class SegmentArcIntersectionBatch : public testing::TestWithParam<SegmentArcIntersectionCase>
{
};
TEST_P(SegmentArcIntersectionBatch, DetectsIntersectionAndCount)
{
    const auto &item = GetParam();
    std::vector<Point> points;
    EXPECT_EQ(intersect(item.segment, item.arc, &points), item.expected);
    EXPECT_EQ(points.size(), item.pointCount);
    EXPECT_EQ(intersect(item.segment, item.arc, nullptr), item.expected);
}
INSTANTIATE_TEST_SUITE_P(SecantTangentSweepEndpointAndPointSegment, SegmentArcIntersectionBatch,
                         testing::Values(SegmentArcIntersectionCase{{{-6, 0}, {6, 0}}, {{0, 0}, 5, 0, 180}, true, 2},
                                         SegmentArcIntersectionCase{{{-6, 5}, {6, 5}}, {{0, 0}, 5, 0, 180}, true, 1},
                                         SegmentArcIntersectionCase{{{-6, -5}, {6, -5}}, {{0, 0}, 5, 0, 180}, false, 0},
                                         SegmentArcIntersectionCase{{{5, 0}, {10, 0}}, {{0, 0}, 5, 0, 90}, true, 1},
                                         SegmentArcIntersectionCase{{{-10, 0}, {-5, 0}}, {{0, 0}, 5, 0, 90}, false, 0},
                                         SegmentArcIntersectionCase{{{0, -6}, {0, 6}}, {{0, 0}, 5, 0, 360}, true, 2},
                                         SegmentArcIntersectionCase{{{5, 0}, {5, 0}}, {{0, 0}, 5, 0, 90}, true, 1},
                                         SegmentArcIntersectionCase{{{4, 0}, {4, 0}}, {{0, 0}, 5, 0, 90}, false, 0}));

struct ArcIntersectionCase
{
    Arc first;
    Arc second;
    bool expected;
    std::size_t pointCount;
};
class ArcIntersectionBatch : public testing::TestWithParam<ArcIntersectionCase>
{
};
TEST_P(ArcIntersectionBatch, DetectsIntersectionAndCount)
{
    const auto &item = GetParam();
    std::vector<Point> points;
    EXPECT_EQ(intersect(item.first, item.second, &points), item.expected);
    EXPECT_EQ(points.size(), item.pointCount);
    EXPECT_EQ(intersect(item.first, item.second, nullptr), item.expected);
}
INSTANTIATE_TEST_SUITE_P(SecantTangentSeparateContainedConcentricAndSweep, ArcIntersectionBatch,
                         testing::Values(ArcIntersectionCase{{{0, 0}, 5, 0, 360}, {{6, 0}, 5, 0, 360}, true, 2},
                                         ArcIntersectionCase{{{0, 0}, 5, 0, 360}, {{10, 0}, 5, 0, 360}, true, 1},
                                         ArcIntersectionCase{{{0, 0}, 5, 0, 360}, {{11, 0}, 5, 0, 360}, false, 0},
                                         ArcIntersectionCase{{{0, 0}, 10, 0, 360}, {{1, 0}, 2, 0, 360}, false, 0},
                                         ArcIntersectionCase{{{0, 0}, 5, 0, 360}, {{0, 0}, 3, 0, 360}, false, 0},
                                         ArcIntersectionCase{{{0, 0}, 5, 0, 90}, {{6, 0}, 5, 0, 90}, false, 0},
                                         ArcIntersectionCase{{{0, 0}, 5, 0, 180}, {{0, 0}, 5, 90, 180}, true, 2}));

struct ArcPointsCase
{
    Arc arc;
    Point expectedStart;
    Point expectedEnd;
};
class ArcPointsBatch : public testing::TestWithParam<ArcPointsCase>
{
};
TEST_P(ArcPointsBatch, CalculatesStartAndEndPoints)
{
    const auto &item = GetParam();
    expectPointNear(startPoint(item.arc), item.expectedStart);
    expectPointNear(endPoint(item.arc), item.expectedEnd);
}
INSTANTIATE_TEST_SUITE_P(CardinalWrappedAndClockwise, ArcPointsBatch,
                         testing::Values(ArcPointsCase{{{0, 0}, 2, 0, 90}, {2, 0}, {0, 2}},
                                         ArcPointsCase{{{1, 1}, 3, 90, 90}, {1, 4}, {-2, 1}},
                                         ArcPointsCase{
                                             {{0, 0}, 4, 300, 120}, {2, -2 * std::sqrt(3.0)}, {2, 2 * std::sqrt(3.0)}},
                                         ArcPointsCase{{{0, 0}, 2, 0, -90}, {2, 0}, {0, -2}},
                                         ArcPointsCase{{{5, -2}, 0, 45, 180}, {5, -2}, {5, -2}}));

TEST(GGeometryThreePointBatch, BuildsRepresentativeArcs)
{
    struct Case
    {
        Point start;
        Point middle;
        Point end;
        Point center;
        double radius;
        double sweep;
    };
    const std::vector<Case> cases{{{1, 0}, {0, 1}, {-1, 0}, {0, 0}, 1, 180},
                                  {{1, 0}, {0, -1}, {-1, 0}, {0, 0}, 1, -180},
                                  {{5, 0}, {0, 5}, {-5, 0}, {0, 0}, 5, 180},
                                  {{3, 2}, {2, 3}, {1, 2}, {2, 2}, 1, 180},
                                  {{2, 0}, {0, -2}, {-2, 0}, {0, 0}, 2, -180}};

    for (const Case &item : cases)
    {
        Arc arc;
        ASSERT_TRUE(arcFromThreePoints(item.start, item.middle, item.end, &arc));
        expectPointNear(arc.center, item.center);
        EXPECT_NEAR(arc.radius, item.radius, Tolerance);
        EXPECT_NEAR(arc.sweepAngle, item.sweep, Tolerance);
        EXPECT_TRUE(contains(arc, item.middle));
        expectPointNear(startPoint(arc), item.start);
        expectPointNear(endPoint(arc), item.end);
    }
}

TEST(GGeometryThreePointBatch, RejectsInvalidInputs)
{
    Arc arc;
    EXPECT_FALSE(arcFromThreePoints({0, 0}, {1, 0}, {2, 0}, &arc));
    EXPECT_FALSE(arcFromThreePoints({0, 0}, {0, 0}, {1, 1}, &arc));
    EXPECT_FALSE(arcFromThreePoints({0, 0}, {1, 1}, {1, 1}, &arc));
    EXPECT_FALSE(arcFromThreePoints({0, 0}, {1, 1}, {0, 0}, &arc));
    EXPECT_FALSE(arcFromThreePoints({0, 0}, {1, 1}, {2, 0}, nullptr));
}

} // namespace
