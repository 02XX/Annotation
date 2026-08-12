#include "View/Main/GMainWindow.hpp"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication app{argc, argv};
    totcad::GMainWindow w{};
    w.show();
    return app.exec();
}