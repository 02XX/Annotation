#include "Tools/Navigation/GWindowZoomTool.hpp"
#include "Graphics/GView.hpp"
#include <QMouseEvent>
namespace totcad {
GWindowZoomTool::GWindowZoomTool(GView *view, QObject *parent) : GTool(view, parent) {}
bool GWindowZoomTool::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return false;
    m_start = event->pos();
    if (!m_rubberBand) m_rubberBand = new QRubberBand(QRubberBand::Rectangle, view()->viewport());
    m_rubberBand->setGeometry(QRect(m_start, QSize()));
    m_rubberBand->show();
    event->accept();
    return true;
}
bool GWindowZoomTool::mouseMove(QMouseEvent *event)
{
    if (!m_rubberBand || !m_rubberBand->isVisible()) return false;
    m_rubberBand->setGeometry(QRect(m_start, event->pos()).normalized());
    event->accept();
    return true;
}
bool GWindowZoomTool::mouseRelease(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !m_rubberBand || !m_rubberBand->isVisible()) return false;
    const QRect rectangle = m_rubberBand->geometry().normalized();
    m_rubberBand->hide();
    if (rectangle.width() > 4 && rectangle.height() > 4)
        view()->zoomToSceneRect(view()->mapToScene(rectangle).boundingRect());
    event->accept();
    return true;
}
void GWindowZoomTool::deactivate()
{
    if (m_rubberBand) m_rubberBand->hide();
    GTool::deactivate();
}
} // namespace totcad
