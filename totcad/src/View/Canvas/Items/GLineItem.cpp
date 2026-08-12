#include "View/Canvas/Items/GLineItem.hpp"

#include "Model/Entities/GLineEntity.hpp"

namespace totcad {

GLineItem::GLineItem(GLineEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GLineItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GLineItem::shape() const { return itemShape(entityPath()); }
void GLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GLineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GLineItem::entityPath() const
{
    QPainterPath path;
    if (m_entity) {
        path.moveTo(m_entity->start.x(), -m_entity->start.y());
        path.lineTo(m_entity->end.x(), -m_entity->end.y());
    }
    return path;
}

} // namespace totcad
