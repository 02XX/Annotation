#include "View/Canvas/Items/GDimensionItem.hpp"

#include "Model/Entities/GDimensionEntity.hpp"

#include <QFont>

namespace totcad {

GDimensionItem::GDimensionItem(GDimensionEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GDimensionItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GDimensionItem::shape() const { return itemShape(entityPath()); }
void GDimensionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GDimensionItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GDimensionItem::entityPath() const
{
    QPainterPath path;
    if (!m_entity)
        return path;
    path.moveTo(m_entity->definitionPoint.x(), -m_entity->definitionPoint.y());
    path.lineTo(m_entity->textPoint.x(), -m_entity->textPoint.y());
    if (!m_entity->text.isEmpty()) {
        QFont font;
        font.setPointSizeF(2.5);
        path.addText(QPointF(m_entity->textPoint.x(), -m_entity->textPoint.y()), font, m_entity->text);
    }
    return path;
}

} // namespace totcad
