#include "Graphics/GView.hpp"

#include "Tools/GTool.hpp"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QtMath>

namespace totcad {

GView::GView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::StrongFocus);
}

void GView::setActiveTool(GTool *tool)
{
    if (m_activeTool == tool)
        return;
    if (m_activeTool)
        m_activeTool->deactivate();
    m_activeTool = tool;
    if (m_activeTool)
        m_activeTool->activate();
}

void GView::showAll()
{
    if (!scene() || scene()->items().isEmpty())
        return;
    zoomToSceneRect(scene()->itemsBoundingRect(), 0.92);
}

void GView::zoomToSceneRect(const QRectF &rectangle, qreal viewportRatio)
{
    if (rectangle.isEmpty())
        return;
    const qreal safeRatio = qBound(0.1, viewportRatio, 1.0);
    fitInView(rectangle, Qt::KeepAspectRatio);
    scale(safeRatio, safeRatio);
    centerOn(rectangle.center());
}

void GView::zoomBy(qreal factor, const QPoint &anchor)
{
    if (factor <= 0.0)
        return;
    const QPoint effectiveAnchor = anchor.isNull() ? viewport()->rect().center() : anchor;
    const QPointF before = mapToScene(effectiveAnchor);
    const qreal current = transform().m11();
    const qreal target = current * factor;
    if (target < 1.0e-4 || target > 1.0e5)
        return;
    scale(factor, factor);
    const QPointF after = mapToScene(effectiveAnchor);
    translate(after.x() - before.x(), after.y() - before.y());
}

void GView::panBy(const QPoint &delta)
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
    verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
}

void GView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        m_middlePanning = true;
        m_middlePanPosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    if (m_activeTool && m_activeTool->mousePress(event))
        return;
    if (event->button() == Qt::LeftButton)
        m_selectionStart = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void GView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseScenePositionChanged(mapToScene(event->pos()));
    if (m_middlePanning) {
        const QPoint delta = event->pos() - m_middlePanPosition;
        panBy(delta);
        m_middlePanPosition = event->pos();
        event->accept();
        return;
    }
    if (m_activeTool && m_activeTool->mouseMove(event))
        return;
    if (event->buttons().testFlag(Qt::LeftButton))
        setRubberBandSelectionMode(event->pos().x() >= m_selectionStart.x()
                                       ? Qt::ContainsItemShape
                                       : Qt::IntersectsItemShape);
    QGraphicsView::mouseMoveEvent(event);
}

void GView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton && m_middlePanning) {
        m_middlePanning = false;
        setCursor(m_activeTool ? m_activeTool->cursor() : Qt::CrossCursor);
        event->accept();
        return;
    }
    if (m_activeTool && m_activeTool->mouseRelease(event))
        return;
    QGraphicsView::mouseReleaseEvent(event);
}

void GView::wheelEvent(QWheelEvent *event)
{
    const qreal factor = qPow(1.0015, static_cast<qreal>(event->angleDelta().y()));
    zoomBy(factor, event->pos());
    event->accept();
}

void GView::keyPressEvent(QKeyEvent *event)
{
    if (m_activeTool && m_activeTool->keyPress(event))
        return;
    QGraphicsView::keyPressEvent(event);
}

} // namespace totcad
