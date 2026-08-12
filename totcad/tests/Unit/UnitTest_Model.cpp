#include "Command/Assignment/GAssignInstanceCommand.hpp"
#include "Command/Assignment/GAssignTypeCommand.hpp"
#include "Command/Type/GDeleteTypeCommand.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"

#include <QUndoStack>
#include <gtest/gtest.h>

namespace {

TEST(GAnnotationDocumentTest, EnforcesTypeAndInstanceRelations)
{
    totcad::GAnnotationDocument document;
    const QString firstType = document.addType(QStringLiteral("Door"), Qt::red);
    const QString secondType = document.addType(QStringLiteral("Window"), Qt::blue);
    const QString instance = document.addInstance(firstType, QStringLiteral("Door1"));

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
    const QString type = document.addType(QStringLiteral("Table"), Qt::green);
    const QString instance = document.addInstance(type, QStringLiteral("Table1"));
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

TEST(GAnnotationDocumentTest, SupportsRepeatedTypeAndInstanceAssignments)
{
    totcad::GAnnotationDocument document;
    const QString type = document.addType(QStringLiteral("Door"), Qt::red);
    const QString instance = document.addInstance(type, QStringLiteral("Door1"));

    document.assignType({QStringLiteral("A")}, type);
    document.assignType({QStringLiteral("B")}, type);
    EXPECT_EQ(document.assignedTypeEntityCount(type), 2);

    document.assignInstance({QStringLiteral("A")}, instance);
    document.assignInstance({QStringLiteral("B")}, instance);
    EXPECT_EQ(document.assignedInstanceEntityCount(instance), 2);
    EXPECT_EQ(document.instanceForEntity(QStringLiteral("A")), instance);
    EXPECT_EQ(document.instanceForEntity(QStringLiteral("B")), instance);
}

TEST(GAnnotationCommandTest, UndoesAndRedoesAssignments)
{
    totcad::GAnnotationDocument document;
    const QString type = document.addType(QStringLiteral("Window"), Qt::blue);
    QUndoStack stack;

    stack.push(new totcad::GAssignTypeCommand(&document,
                                              {QStringLiteral("10"), QStringLiteral("11")},
                                              type));
    EXPECT_EQ(document.assignedTypeEntityCount(type), 2);

    stack.undo();
    EXPECT_EQ(document.assignedTypeEntityCount(type), 0);

    stack.redo();
    EXPECT_EQ(document.assignedTypeEntityCount(type), 2);
}

} // namespace
