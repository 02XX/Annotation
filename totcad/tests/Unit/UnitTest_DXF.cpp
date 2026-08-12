#include "DXF/Reader.hpp"

#include <gtest/gtest.h>

#include <sstream>

namespace {

TEST(DXFReaderTest, ReadsSectionsTablesBlocksAndEntities)
{
    std::istringstream input{
        "0\nSECTION\n2\nHEADER\n9\n$ACADVER\n1\nAC1009\n0\nENDSEC\n"
        "0\nSECTION\n2\nTABLES\n0\nTABLE\n2\nLAYER\n70\n1\n"
        "0\nLAYER\n2\nWALL\n62\n1\n6\nCONTINUOUS\n0\nENDTAB\n0\nENDSEC\n"
        "0\nSECTION\n2\nBLOCKS\n0\nBLOCK\n2\nDOOR\n10\n0\n20\n0\n"
        "0\nLINE\n10\n0\n20\n0\n11\n1\n21\n0\n0\nENDBLK\n0\nENDSEC\n"
        "0\nSECTION\n2\nENTITIES\n0\nLINE\n5\nA1\n8\nWALL\n"
        "10\n1\n20\n2\n11\n4\n21\n6\n0\nENDSEC\n0\nEOF\n"};

    totcad::dxf::DXFDocument document;
    std::string error;
    ASSERT_TRUE(totcad::dxf::Reader{}.read(input, document, &error)) << error;

    ASSERT_NE(document.findTable("layer"), nullptr);
    ASSERT_EQ(document.findTable("LAYER")->records.size(), 1U);
    EXPECT_EQ(document.findTable("LAYER")->records.front().stringValue(2), "WALL");
    ASSERT_EQ(document.blocks.size(), 1U);
    EXPECT_EQ(document.blocks.front().definition.stringValue(2), "DOOR");
    ASSERT_EQ(document.entities.size(), 1U);
    EXPECT_EQ(document.entities.front().type, "LINE");
    EXPECT_DOUBLE_EQ(document.entities.front().doubleValue(21).value_or(0.0), 6.0);
}

TEST(DXFReaderTest, DoesNotChangeDocumentAfterAnError)
{
    totcad::dxf::DXFDocument document;
    document.entities.push_back({"KEEP", {}});
    std::istringstream input{"0\nSECTION\n2\nENTITIES\n10\n"};
    std::string error;

    EXPECT_FALSE(totcad::dxf::Reader{}.read(input, document, &error));
    ASSERT_EQ(document.entities.size(), 1U);
    EXPECT_EQ(document.entities.front().type, "KEEP");
    EXPECT_FALSE(error.empty());
}

} // namespace
