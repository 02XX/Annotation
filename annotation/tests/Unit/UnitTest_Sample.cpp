#include "UI/GAnnotationWindow.hpp"

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

TEST(GAnnotationWindowTest, UsesExpectedInitialSize)
{
    application();
    annotation::GAnnotationWindow window;

    EXPECT_EQ(window.width(), 1920);
    EXPECT_EQ(window.height(), 1080);
}
