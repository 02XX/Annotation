#include "IO/GAnnotationSerializer.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"

#include <QTemporaryDir>
#include <gtest/gtest.h>

namespace {

TEST(GAnnotationSerializerTest, RoundTripsAnnotationRelations)
{
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    const QString filePath = directory.filePath(QStringLiteral("drawing.json"));

    totcad::GAnnotationDocument source;
    const QString type = source.addType(QStringLiteral("沙发"), QColor(QStringLiteral("#12AB34")));
    const QString instance = source.addInstance(type, QStringLiteral("沙发1"));
    source.assignInstance({QStringLiteral("2A"), QStringLiteral("2B")}, instance);

    QString error;
    totcad::GAnnotationSerializer serializer;
    ASSERT_TRUE(serializer.save(filePath, source, &error)) << error.toStdString();

    totcad::GAnnotationDocument loaded;
    ASSERT_TRUE(serializer.load(filePath, loaded, &error)) << error.toStdString();
    ASSERT_NE(loaded.type(type), nullptr);
    ASSERT_NE(loaded.instance(instance), nullptr);
    EXPECT_EQ(loaded.type(type)->color, QColor(QStringLiteral("#12AB34")));
    EXPECT_EQ(loaded.instanceForEntity(QStringLiteral("2A")), instance);
    EXPECT_EQ(loaded.typeForEntity(QStringLiteral("2B")), type);
    EXPECT_FALSE(loaded.isDirty());
}

} // namespace
