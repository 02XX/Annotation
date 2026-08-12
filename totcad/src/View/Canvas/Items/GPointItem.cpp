#include "View/Canvas/Items/GPointItem.hpp"

#include "Model/Entities/GPointEntity.hpp"

namespace totcad {

GPointItem::GPointItem(GPointEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GPointItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GPointItem::shape() const { return itemShape(entityPath()); }
void GPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GPointItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GPointItem::entityPath() const
{
    QPainterPath path;
    if (m_entity)
        path.addEllipse(QPointF(m_entity->position.x(), -m_entity->position.y()), 1.5, 1.5);
    return path;
}

} // namespace totcad
