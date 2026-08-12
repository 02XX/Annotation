#include "View/Canvas/Items/GArcItem.hpp"

#include "Model/Entities/GArcEntity.hpp"

namespace totcad {

namespace {
double positiveSweep(double start, double end)
{
    double result = end - start;
    while (result <= 0.0)
        result += 360.0;
    return result;
}
}

GArcItem::GArcItem(GArcEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GArcItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GArcItem::shape() const { return itemShape(entityPath()); }
void GArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GArcItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GArcItem::entityPath() const
{
    QPainterPath path;
    if (!m_entity)
        return path;
    const QRectF rectangle(m_entity->center.x() - m_entity->radius,
                           -m_entity->center.y() - m_entity->radius,
                           m_entity->radius * 2.0,
                           m_entity->radius * 2.0);
    path.arcMoveTo(rectangle, -m_entity->startAngle);
    path.arcTo(rectangle,
               -m_entity->startAngle,
               -positiveSweep(m_entity->startAngle, m_entity->endAngle));
    return path;
}

} // namespace totcad
