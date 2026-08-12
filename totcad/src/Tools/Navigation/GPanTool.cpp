#include "Tools/Navigation/GPanTool.hpp"
#include "Graphics/GView.hpp"
#include <QMouseEvent>
namespace totcad {
GPanTool::GPanTool(GView *view, QObject *parent) : GTool(view, parent) {}
bool GPanTool::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return false;
    m_panning = true;
    m_lastPosition = event->pos();
    view()->setCursor(Qt::ClosedHandCursor);
    event->accept();
    return true;
}
bool GPanTool::mouseMove(QMouseEvent *event)
{
    if (!m_panning) return false;
    view()->panBy(event->pos() - m_lastPosition);
    m_lastPosition = event->pos();
    event->accept();
    return true;
}
bool GPanTool::mouseRelease(QMouseEvent *event)
{
    if (!m_panning || event->button() != Qt::LeftButton) return false;
    m_panning = false;
    view()->setCursor(Qt::OpenHandCursor);
    event->accept();
    return true;
}
} // namespace totcad
