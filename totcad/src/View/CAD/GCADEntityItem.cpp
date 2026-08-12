#include "View/CAD/GCADEntityItem.hpp"

namespace totcad {

GCADEntityItem::GCADEntityItem(QString entityId, QString layerName, const QPainterPath &path)
    : QGraphicsPathItem(path), m_entityId(std::move(entityId)), m_layerName(std::move(layerName))
{
    setFlags(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setZValue(0.0);
}

void GCADEntityItem::setBaseStyle(const QPen &penValue, const QBrush &brushValue)
{
    m_basePen = penValue;
    m_baseBrush = brushValue;
    updateSelectionStyle(isSelected());
}

QVariant GCADEntityItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged)
        updateSelectionStyle(value.toBool());
    return QGraphicsPathItem::itemChange(change, value);
}

void GCADEntityItem::updateSelectionStyle(bool selected)
{
    if (selected) {
        QPen selectedPen(QColor(255, 210, 40), 2.0);
        selectedPen.setCosmetic(true);
        setPen(selectedPen);
        setBrush(m_baseBrush);
    } else {
        QPen normalPen = m_basePen;
        normalPen.setCosmetic(true);
        setPen(normalPen);
        setBrush(m_baseBrush);
    }
}

} // namespace totcad
