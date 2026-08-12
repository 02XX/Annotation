#include "DXF/GDXFParser.hpp"
#include "Model/Entities/GCircleEntity.hpp"
#include "Model/Entities/GDocumentEntity.hpp"
#include "Model/Entities/GLineEntity.hpp"

#include <gtest/gtest.h>

namespace {

TEST(GDXFParserTest, ParsesLayersAndBasicEntities)
{
    const QVector<totcad::GDXFGroup> groups{
        {0, QStringLiteral("SECTION"), 1}, {2, QStringLiteral("TABLES"), 3},
        {0, QStringLiteral("LAYER"), 5}, {2, QStringLiteral("WALL"), 7},
        {62, QStringLiteral("1"), 9}, {6, QStringLiteral("CONTINUOUS"), 11},
        {0, QStringLiteral("ENDSEC"), 13},
        {0, QStringLiteral("SECTION"), 15}, {2, QStringLiteral("ENTITIES"), 17},
        {0, QStringLiteral("LINE"), 19}, {5, QStringLiteral("A1"), 21},
        {8, QStringLiteral("WALL"), 23}, {10, QStringLiteral("1"), 25},
        {20, QStringLiteral("2"), 27}, {11, QStringLiteral("4"), 29},
        {21, QStringLiteral("6"), 31},
        {0, QStringLiteral("CIRCLE"), 33}, {5, QStringLiteral("A2"), 35},
        {10, QStringLiteral("10"), 37}, {20, QStringLiteral("20"), 39},
        {40, QStringLiteral("5"), 41}, {0, QStringLiteral("ENDSEC"), 43},
        {0, QStringLiteral("EOF"), 45}
    };

    totcad::GDocumentEntity document;
    QString error;
    ASSERT_TRUE(totcad::GDXFParser{}.parse(groups, document, &error)) << error.toStdString();
    ASSERT_EQ(document.entityCount(), 2);
    EXPECT_TRUE(document.layers().contains(QStringLiteral("WALL")));
    EXPECT_NE(std::dynamic_pointer_cast<totcad::GLineEntity>(document.entity(0xA1)), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<totcad::GCircleEntity>(document.entity(0xA2)), nullptr);
}

} // namespace
