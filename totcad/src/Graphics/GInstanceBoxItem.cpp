#include "Graphics/GInstanceBoxItem.hpp"

#include <QPen>

namespace totcad {

GInstanceBoxItem::GInstanceBoxItem(QString instanceId, const QRectF &rectangle, const QColor &color)
    : QGraphicsRectItem(rectangle), m_instanceId(std::move(instanceId))
{
    QPen boxPen(color, 2.0, Qt::DashLine);
    boxPen.setCosmetic(true);
    setPen(boxPen);
    setBrush(Qt::NoBrush);
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(10.0);
}

} // namespace totcad
