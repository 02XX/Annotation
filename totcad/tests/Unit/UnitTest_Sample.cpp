#include "View/Main/GMainWindow.hpp"

#include <QApplication>
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
