#include "Graphics/GPolylineItem.hpp"

#include "Model/Entities/GPolylineEntity.hpp"

#include <QLineF>
#include <QtMath>

namespace totcad {

namespace {
QPointF scenePoint(const QPointF &point) { return {point.x(), -point.y()}; }

void appendBulgedSegment(QPainterPath &path, const QPointF &start, const QPointF &end, double bulge)
{
    if (qAbs(bulge) < 1.0e-9) {
        path.lineTo(scenePoint(end));
        return;
    }
    const double chord = QLineF(start, end).length();
    if (chord < 1.0e-9)
        return;
    const QPointF midpoint = (start + end) / 2.0;
    const QPointF direction = (end - start) / chord;
    const QPointF leftNormal(-direction.y(), direction.x());
    const double centerDistance = chord * (1.0 - bulge * bulge) / (4.0 * bulge);
    const QPointF center = midpoint + leftNormal * centerDistance;
    const double radius = QLineF(center, start).length();
    const double startAngle = qAtan2(start.y() - center.y(), start.x() - center.x());
    const double sweep = 4.0 * qAtan(bulge);
    const int segments = qMax(4, static_cast<int>(qCeil(qAbs(sweep) / (M_PI / 18.0))));
    for (int step = 1; step <= segments; ++step) {
        const double angle = startAngle + sweep * static_cast<double>(step) / static_cast<double>(segments);
        path.lineTo(scenePoint(center + QPointF(qCos(angle) * radius, qSin(angle) * radius)));
    }
}
}

GPolylineItem::GPolylineItem(GPolylineEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GPolylineItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GPolylineItem::shape() const { return itemShape(entityPath()); }
void GPolylineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GPolylineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GPolylineItem::entityPath() const
{
    QPainterPath path;
    if (!m_entity || m_entity->vertices.isEmpty())
        return path;
    path.moveTo(scenePoint(m_entity->vertices.first()));
    for (int i = 1; i < m_entity->vertices.size(); ++i) {
        const double bulge = i - 1 < m_entity->bulges.size() ? m_entity->bulges.at(i - 1) : 0.0;
        appendBulgedSegment(path, m_entity->vertices.at(i - 1), m_entity->vertices.at(i), bulge);
    }
    if (m_entity->closed) {
        const int last = m_entity->vertices.size() - 1;
        const double bulge = last < m_entity->bulges.size() ? m_entity->bulges.at(last) : 0.0;
        appendBulgedSegment(path, m_entity->vertices.last(), m_entity->vertices.first(), bulge);
        path.closeSubpath();
    }
    return path;
}

} // namespace totcad
