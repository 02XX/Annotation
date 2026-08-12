#include "IO/Annotation/GAnnotationSerializer.hpp"
#include "Model/GAnnotationModel.hpp"

#include <QTemporaryDir>
#include <gtest/gtest.h>

namespace {

TEST(GAnnotationSerializerTest, RoundTripsAnnotationRelations)
{
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    const QString filePath = directory.filePath(QStringLiteral("drawing.json"));

    totcad::GAnnotationModel source;
    const QString type = source.addType(QStringLiteral("沙发"), QColor(QStringLiteral("#12AB34")));
    const QString instance = source.addInstance(type, QStringLiteral("沙发1"));
    constexpr totcad::EntityID firstEntityId = UINT64_C(9007199254740993);
    constexpr totcad::EntityID secondEntityId = UINT64_C(9007199254740994);
    source.assignInstance({firstEntityId, secondEntityId}, instance);

    QString error;
    totcad::GAnnotationSerializer serializer;
    ASSERT_TRUE(serializer.save(filePath, source, &error)) << error.toStdString();

    totcad::GAnnotationModel loaded;
    ASSERT_TRUE(serializer.load(filePath, loaded, &error)) << error.toStdString();
    ASSERT_NE(loaded.type(type), nullptr);
    ASSERT_NE(loaded.instance(instance), nullptr);
    EXPECT_EQ(loaded.type(type)->color, QColor(QStringLiteral("#12AB34")));
    EXPECT_EQ(loaded.instanceForEntity(firstEntityId), instance);
    EXPECT_EQ(loaded.typeForEntity(secondEntityId), type);
    EXPECT_FALSE(loaded.isDirty());
}

} // namespace
