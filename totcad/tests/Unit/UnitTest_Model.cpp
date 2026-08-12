#include "Command/Assignment/GAssignInstanceCommand.hpp"
#include "Command/Type/GDeleteTypeCommand.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"

#include <QUndoStack>
#include <gtest/gtest.h>

namespace {

TEST(GAnnotationDocumentTest, EnforcesTypeAndInstanceRelations)
{
    totcad::GAnnotationDocument document;
    const QString firstType = document.addType(QStringLiteral("门"), Qt::red);
    const QString secondType = document.addType(QStringLiteral("窗"), Qt::blue);
    const QString instance = document.addInstance(firstType, QStringLiteral("门1"));

    document.assignInstance({QStringLiteral("10"), QStringLiteral("11")}, instance);
    EXPECT_EQ(document.typeForEntity(QStringLiteral("10")), firstType);
    EXPECT_EQ(document.instanceForEntity(QStringLiteral("10")), instance);

    document.assignType({QStringLiteral("10")}, secondType);
    EXPECT_EQ(document.typeForEntity(QStringLiteral("10")), secondType);
    EXPECT_TRUE(document.instanceForEntity(QStringLiteral("10")).isEmpty());
    ASSERT_NE(document.instance(instance), nullptr);
    EXPECT_FALSE(document.instance(instance)->entityIds.contains(QStringLiteral("10")));
}

TEST(GAnnotationCommandTest, RestoresCascadeDeletion)
{
    totcad::GAnnotationDocument document;
    const QString type = document.addType(QStringLiteral("桌子"), Qt::green);
    const QString instance = document.addInstance(type, QStringLiteral("桌子1"));
    document.assignInstance({QStringLiteral("AB")}, instance);

    QUndoStack stack;
    stack.push(new totcad::GDeleteTypeCommand(&document, type));
    EXPECT_EQ(document.types().size(), 0);
    EXPECT_TRUE(document.instanceForEntity(QStringLiteral("AB")).isEmpty());

    stack.undo();
    EXPECT_NE(document.type(type), nullptr);
    EXPECT_NE(document.instance(instance), nullptr);
    EXPECT_EQ(document.instanceForEntity(QStringLiteral("AB")), instance);
}

} // namespace
