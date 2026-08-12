#include "View/Canvas/Items/GCircleItem.hpp"

#include "Model/Entities/GCircleEntity.hpp"

namespace totcad {

GCircleItem::GCircleItem(GCircleEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GCircleItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GCircleItem::shape() const { return itemShape(entityPath()); }
void GCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GCircleItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GCircleItem::entityPath() const
{
    QPainterPath path;
    if (m_entity) {
        path.addEllipse(QRectF(m_entity->center.x() - m_entity->radius,
                               -m_entity->center.y() - m_entity->radius,
                               m_entity->radius * 2.0,
                               m_entity->radius * 2.0));
    }
    return path;
}

} // namespace totcad
