#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QBrush>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPen>

namespace totcad {

// Shared selection and painting behavior for canvas entity items.
class GEntityItem
{
public:
    GEntityItem(QGraphicsItem *graphicsItem, EntityID entityId, QString layerName);
    virtual ~GEntityItem() = default;

    EntityID entityId() const noexcept { return m_entityId; }
    void setSelectionEntityId(EntityID entityId) noexcept { m_entityId = entityId; }
    const QString &layerName() const noexcept { return m_layerName; }
    void setBaseStyle(const QPen &pen, const QBrush &brush);

protected:
    QRectF itemBoundingRect(const QPainterPath &path) const;
    QPainterPath itemShape(const QPainterPath &path) const;
    void paintItem(QPainter *painter, const QPainterPath &path) const;
    void selectionChanged();

private:
    QPen currentPen() const;

    QGraphicsItem *m_graphicsItem{nullptr};
    EntityID m_entityId{};
    QString m_layerName;
    QPen m_basePen;
    QBrush m_baseBrush;
};

} // namespace totcad
