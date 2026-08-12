#include "UI/GAnnotationWindow.hpp"

#include "Graphics/GGraphicsScene.hpp"
#include "Graphics/GGraphicsView.hpp"

namespace annotation
{
GAnnotationWindow::GAnnotationWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1920, 1080);
    m_graphicsScene = new GGraphicsScene(this);
    m_graphicsView = new GGraphicsView(m_graphicsScene, this);
    m_graphicsView->setBackgroundBrush(Qt::black);
    setCentralWidget(m_graphicsView);

    m_graphicsScene->addEllipse(100, 100, 80, 80, QPen(Qt::green));
}
} // namespace annotation
