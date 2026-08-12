#include "Command/Assignment/GAssignInstanceCommand.hpp"
#include "Command/Type/GDeleteTypeCommand.hpp"
#include "Model/GAnnotationModel.hpp"
#include "Model/GDXFModel.hpp"
#include "Model/Entities/GLineEntity.hpp"

#include <QUndoStack>
#include <gtest/gtest.h>
#include <memory>

namespace {

TEST(GEntityTest, StoresTypedEntityId)
{
    totcad::GEntity entity(0);
    entity.setId(42);

    const totcad::IEntity<totcad::EntityID> *interfaceEntity = &entity;
    EXPECT_NE(interfaceEntity, nullptr);
    EXPECT_EQ(entity.id(), 42U);
}

TEST(GModelTest, DrawingAndAnnotationsHaveIndependentLifecycles)
{
    totcad::GDXFModel drawing;
    totcad::GAnnotationModel annotations;
    EXPECT_TRUE(drawing.isEmpty());
    EXPECT_TRUE(annotations.isEmpty());

    drawing.addEntity(std::make_shared<totcad::GLineEntity>(1));
    annotations.addType(QStringLiteral("门"), Qt::red);
    EXPECT_FALSE(drawing.isEmpty());
    EXPECT_FALSE(annotations.isEmpty());

    drawing.clear();
    annotations.clear();
    EXPECT_TRUE(drawing.isEmpty());
    EXPECT_TRUE(annotations.isEmpty());
}

TEST(GAnnotationModelTest, EnforcesTypeAndInstanceRelations)
{
    totcad::GAnnotationModel document;
    const QString firstType = document.addType(QStringLiteral("门"), Qt::red);
    const QString secondType = document.addType(QStringLiteral("窗"), Qt::blue);
    const QString instance = document.addInstance(firstType, QStringLiteral("门1"));

    document.assignInstance({10, 11}, instance);
    EXPECT_EQ(document.typeForEntity(10), firstType);
    EXPECT_EQ(document.instanceForEntity(10), instance);

    document.assignType({10}, secondType);
    EXPECT_EQ(document.typeForEntity(10), secondType);
    EXPECT_TRUE(document.instanceForEntity(10).isEmpty());
    ASSERT_NE(document.instance(instance), nullptr);
    EXPECT_FALSE(document.instance(instance)->entityIds.contains(10));
}

TEST(GAnnotationCommandTest, RestoresCascadeDeletion)
{
    totcad::GAnnotationModel document;
    const QString type = document.addType(QStringLiteral("桌子"), Qt::green);
    const QString instance = document.addInstance(type, QStringLiteral("桌子1"));
    document.assignInstance({0xAB}, instance);

    QUndoStack stack;
    stack.push(new totcad::GDeleteTypeCommand(&document, type));
    EXPECT_EQ(document.types().size(), 0);
    EXPECT_TRUE(document.instanceForEntity(0xAB).isEmpty());

    stack.undo();
    EXPECT_NE(document.type(type), nullptr);
    EXPECT_NE(document.instance(instance), nullptr);
    EXPECT_EQ(document.instanceForEntity(0xAB), instance);
}

} // namespace
