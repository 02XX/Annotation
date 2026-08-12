#include "View/Main/GMainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QTimer>
#include <gtest/gtest.h>

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
