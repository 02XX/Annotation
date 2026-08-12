#include "View/Canvas/Tools/Navigation/GRealtimeZoomTool.hpp"
#include "View/Canvas/GGraphicsView.hpp"
#include <QMouseEvent>
#include <QtMath>
namespace totcad {
GRealtimeZoomTool::GRealtimeZoomTool(GGraphicsView *view, QObject *parent) : GCanvasTool(view, parent) {}
bool GRealtimeZoomTool::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return false;
    m_zooming = true;
    m_lastPosition = event->pos();
    event->accept();
    return true;
}
bool GRealtimeZoomTool::mouseMove(QMouseEvent *event)
{
    if (!m_zooming) return false;
    const int delta = m_lastPosition.y() - event->pos().y();
    view()->zoomBy(qPow(1.01, static_cast<qreal>(delta)), event->pos());
    m_lastPosition = event->pos();
    event->accept();
    return true;
}
bool GRealtimeZoomTool::mouseRelease(QMouseEvent *event)
{
    if (!m_zooming || event->button() != Qt::LeftButton) return false;
    m_zooming = false;
    event->accept();
    return true;
}
} // namespace totcad
