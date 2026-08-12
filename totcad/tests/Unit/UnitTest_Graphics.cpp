#include "View/Canvas/Items/GArcItem.hpp"
#include "View/Canvas/Items/GCircleItem.hpp"
#include "View/Canvas/Items/GDimensionItem.hpp"
#include "View/Canvas/Items/GHatchItem.hpp"
#include "View/Canvas/Items/GInsertItem.hpp"
#include "View/Canvas/Items/GLineItem.hpp"
#include "View/Canvas/Items/GPointItem.hpp"
#include "View/Canvas/Items/GPolylineItem.hpp"
#include "View/Canvas/Items/GTextItem.hpp"
#include "Model/Entities/GArcEntity.hpp"
#include "Model/Entities/GCircleEntity.hpp"
#include "Model/Entities/GDimensionEntity.hpp"
#include "Model/Entities/GHatchEntity.hpp"
#include "Model/Entities/GInsertEntity.hpp"
#include "Model/Entities/GLineEntity.hpp"
#include "Model/Entities/GPointEntity.hpp"
#include "Model/Entities/GPolylineEntity.hpp"
#include "Model/Entities/GTextEntity.hpp"

#include <gtest/gtest.h>

TEST(GEntityItemTest, KeepsMatchingTypedEntityPointers)
{
    totcad::GPointEntity point(1);
    totcad::GLineEntity line(2);
    totcad::GCircleEntity circle(3);
    totcad::GArcEntity arc(4);
    totcad::GPolylineEntity polyline(5);
    totcad::GTextEntity text(6);
    totcad::GDimensionEntity dimension(7);
    totcad::GHatchEntity hatch(8);
    totcad::GInsertEntity insert(9);

    totcad::GPointItem pointItem(&point);
    totcad::GLineItem lineItem(&line);
    totcad::GCircleItem circleItem(&circle);
    totcad::GArcItem arcItem(&arc);
    totcad::GPolylineItem polylineItem(&polyline);
    totcad::GTextItem textItem(&text);
    totcad::GDimensionItem dimensionItem(&dimension);
    totcad::GHatchItem hatchItem(&hatch);
    totcad::GInsertItem insertItem(&insert);

    EXPECT_EQ(pointItem.entity(), &point);
    EXPECT_EQ(lineItem.entity(), &line);
    EXPECT_EQ(circleItem.entity(), &circle);
    EXPECT_EQ(arcItem.entity(), &arc);
    EXPECT_EQ(polylineItem.entity(), &polyline);
    EXPECT_EQ(textItem.entity(), &text);
    EXPECT_EQ(dimensionItem.entity(), &dimension);
    EXPECT_EQ(hatchItem.entity(), &hatch);
    EXPECT_EQ(insertItem.entity(), &insert);
    EXPECT_EQ(lineItem.entityId(), line.id());
}
