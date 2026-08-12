#include "Command/Assignment/GAssignTypeCommand.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"
#include "Model/CAD/GCADDocument.hpp"
#include "Model/CAD/GCADLine.hpp"
#include "Model/GInstanceTreeModel.hpp"
#include "Model/GTypeTableModel.hpp"
#include "View/CAD/GCADScene.hpp"
#include "View/Dock/GInstanceDockWidget.hpp"
#include "View/Dock/GTypeDockWidget.hpp"
#include "View/Main/GMainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QTimer>
#include <QUndoStack>
#include <gtest/gtest.h>
#include <memory>

namespace
{
QApplication &application()
{
    static int argc = 1;
    static char applicationName[] = "AnnotationUnitTests";
    static char *argv[] = {applicationName, nullptr};
    static QApplication instance{argc, argv};
    return instance;
}
} // namespace

TEST(GMainWindowTest, UsesExpectedInitialSize)
{
    application();
    totcad::GMainWindow window;

    EXPECT_EQ(window.width(), 1440);
    EXPECT_EQ(window.height(), 900);
}

TEST(GMainWindowTest, ProvidesVisibleOpenFileAction)
{
    application();
    totcad::GMainWindow window;

    const auto *openAction = window.findChild<QAction *>(QStringLiteral("OpenFileAction"));
    ASSERT_NE(openAction, nullptr);
    EXPECT_TRUE(openAction->isEnabled());
    EXPECT_EQ(openAction->shortcut(), QKeySequence::Open);
}

TEST(GMainWindowTest, OpenFileActionShowsFileDialog)
{
    application();
    totcad::GMainWindow window;
    window.show();
    auto *openAction = window.findChild<QAction *>(QStringLiteral("OpenFileAction"));
    ASSERT_NE(openAction, nullptr);

    bool dialogWasShown = false;
    QTimer::singleShot(50, [&dialogWasShown] {
        for (QWidget *widget : QApplication::topLevelWidgets()) {
            auto *dialog = qobject_cast<QFileDialog *>(widget);
            if (dialog && dialog->objectName() == QStringLiteral("OpenDXFFileDialog")) {
                dialogWasShown = dialog->isVisible();
                dialog->reject();
                return;
            }
        }
    });

    openAction->trigger();
    EXPECT_TRUE(dialogWasShown);
}

TEST(GAnnotationDockTest, KeepsCurrentTypeAcrossAssignmentsAndUndo)
{
    application();
    totcad::GAnnotationDocument document;
    const QString firstType = document.addType(QStringLiteral("Door"), Qt::red);
    const QString secondType = document.addType(QStringLiteral("Window"), Qt::blue);
    QUndoStack undoStack;
    totcad::GTypeTableModel model(&document, &undoStack);
    totcad::GTypeDockWidget dock;
    dock.setModel(&model);
    dock.selectRow(1);
    ASSERT_EQ(dock.currentTypeId(), secondType);

    undoStack.push(new totcad::GAssignTypeCommand(&document,
                                                  {QStringLiteral("E1")},
                                                  secondType));
    EXPECT_EQ(dock.currentTypeId(), secondType);
    EXPECT_EQ(document.typeForEntity(QStringLiteral("E1")), secondType);

    undoStack.undo();
    EXPECT_EQ(dock.currentTypeId(), secondType);
    EXPECT_TRUE(document.typeForEntity(QStringLiteral("E1")).isEmpty());

    document.setTypeColor(secondType, Qt::green);
    EXPECT_EQ(dock.currentTypeId(), secondType);
    EXPECT_NE(firstType, secondType);
}

TEST(GCADSceneTest, SelectsEntitiesByPersistedIds)
{
    application();
    totcad::GCADDocument cadDocument;
    auto line = std::make_shared<totcad::GCADLine>();
    line->setId(QStringLiteral("LINE-1"));
    line->start = QPointF(0.0, 0.0);
    line->end = QPointF(10.0, 10.0);
    cadDocument.addEntity(line);

    totcad::GAnnotationDocument annotationDocument;
    totcad::GCADScene scene;
    scene.setDocuments(&cadDocument, &annotationDocument);
    scene.selectEntityIds({QStringLiteral("LINE-1")});

    EXPECT_EQ(scene.selectedEntityIds(), QStringList{QStringLiteral("LINE-1")});
}

TEST(GAnnotationDockTest, KeepsCurrentInstanceAcrossAssignmentsAndRename)
{
    application();
    totcad::GAnnotationDocument document;
    const QString type = document.addType(QStringLiteral("Door"), Qt::red);
    const QString firstInstance = document.addInstance(type, QStringLiteral("Door1"));
    const QString secondInstance = document.addInstance(type, QStringLiteral("Door2"));
    QUndoStack undoStack;
    totcad::GInstanceTreeModel model(&document, &undoStack);
    model.setCurrentType(type);
    totcad::GInstanceDockWidget dock;
    dock.setModel(&model);
    dock.selectInstance(secondInstance);
    ASSERT_EQ(dock.currentInstanceId(), secondInstance);

    document.assignInstance({QStringLiteral("E1")}, secondInstance);
    EXPECT_EQ(dock.currentInstanceId(), secondInstance);

    document.setInstanceName(secondInstance, QStringLiteral("Door2-Renamed"));
    EXPECT_EQ(dock.currentInstanceId(), secondInstance);
    EXPECT_NE(firstInstance, secondInstance);
}
