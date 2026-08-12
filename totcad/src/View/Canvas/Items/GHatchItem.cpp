#include "View/Canvas/Items/GHatchItem.hpp"

#include "Model/Entities/GHatchEntity.hpp"

namespace totcad {

GHatchItem::GHatchItem(GHatchEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GHatchItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GHatchItem::shape() const { return itemShape(entityPath()); }
void GHatchItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GHatchItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GHatchItem::entityPath() const
{
    QPainterPath path;
    if (!m_entity)
        return path;
    path.setFillRule(Qt::OddEvenFill);
    for (const auto &boundary : m_entity->boundaries) {
        QPolygonF mapped;
        for (const QPointF &point : boundary)
            mapped.append(QPointF(point.x(), -point.y()));
        path.addPolygon(mapped);
    }
    return path;
}

} // namespace totcad
