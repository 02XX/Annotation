#include "Graphics/GTextItem.hpp"

#include "Model/Entities/GTextEntity.hpp"

#include <QFont>

namespace totcad {

GTextItem::GTextItem(GTextEntity *entity)
    : QGraphicsItem(),
      GEntityItem(this, entity ? entity->id() : EntityID{}, entity ? entity->layerName : QString{}),
      m_entity(entity)
{
}

QRectF GTextItem::boundingRect() const { return itemBoundingRect(entityPath()); }
QPainterPath GTextItem::shape() const { return itemShape(entityPath()); }
void GTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) { paintItem(painter, entityPath()); }
QVariant GTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) selectionChanged();
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath GTextItem::entityPath() const
{
    QPainterPath path;
    if (!m_entity)
        return path;
    QFont font;
    font.setPointSizeF(qMax(1.0, m_entity->height));
    path.addText(QPointF(m_entity->position.x(), -m_entity->position.y()), font, m_entity->text);
    QTransform rotation;
    rotation.translate(m_entity->position.x(), -m_entity->position.y());
    rotation.rotate(-m_entity->rotation);
    rotation.translate(-m_entity->position.x(), m_entity->position.y());
    return rotation.map(path);
}

} // namespace totcad
