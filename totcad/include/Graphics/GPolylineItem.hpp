#pragma once

#include "Graphics/GEntityItem.hpp"

namespace totcad {

class GPolylineEntity;

class GPolylineItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GPolylineItem(GPolylineEntity *entity);
    GPolylineEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GPolylineEntity *m_entity{nullptr};
};

} // namespace totcad
