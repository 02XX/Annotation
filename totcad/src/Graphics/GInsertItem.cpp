#include "Graphics/GInsertItem.hpp"

#include "Model/Entities/GInsertEntity.hpp"

namespace totcad {

GInsertItem::GInsertItem(GInsertEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

QRectF GInsertItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GInsertItem::shape() const { return itemShape(entityPath()); }
void GInsertItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GInsertItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GInsertItem::entityPath() const
{
    return {};
}

} // namespace totcad
