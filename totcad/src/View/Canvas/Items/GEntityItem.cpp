#include "View/Canvas/Items/GEntityItem.hpp"

#include <QPainter>
#include <QPainterPathStroker>

namespace totcad {

GEntityItem::GEntityItem(QGraphicsItem *graphicsItem, EntityID entityId, QString layerName)
    : m_graphicsItem(graphicsItem), m_entityId(entityId), m_layerName(std::move(layerName))
{
    m_graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
    m_graphicsItem->setAcceptHoverEvents(true);
    m_graphicsItem->setZValue(0.0);
}

void GEntityItem::setBaseStyle(const QPen &pen, const QBrush &brush)
{
    m_basePen = pen;
    m_baseBrush = brush;
    m_graphicsItem->update();
}

QRectF GEntityItem::itemBoundingRect(const QPainterPath &path) const
{
    const qreal width = qMax<qreal>(2.0, currentPen().widthF());
    return path.boundingRect().adjusted(-width, -width, width, width);
}

QPainterPath GEntityItem::itemShape(const QPainterPath &path) const
{
    QPainterPath result = path;
    QPainterPathStroker stroker;
    stroker.setWidth(qMax<qreal>(6.0, currentPen().widthF()));
    result.addPath(stroker.createStroke(path));
    return result;
}

void GEntityItem::paintItem(QPainter *painter, const QPainterPath &path) const
{
    painter->setPen(currentPen());
    painter->setBrush(m_baseBrush);
    painter->drawPath(path);
}

void GEntityItem::selectionChanged()
{
    m_graphicsItem->update();
}

QPen GEntityItem::currentPen() const
{
    if (!m_graphicsItem->isSelected()) {
        QPen pen = m_basePen;
        pen.setCosmetic(true);
        return pen;
    }
    QPen pen(QColor(255, 210, 40), 2.0);
    pen.setCosmetic(true);
    return pen;
}

} // namespace totcad
