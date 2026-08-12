#pragma once

#include "View/Canvas/Items/GEntityItem.hpp"

namespace totcad {

class GPointEntity;

class GPointItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GPointItem(GPointEntity *entity);
    GPointEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GPointEntity *m_entity{nullptr};
};

} // namespace totcad
