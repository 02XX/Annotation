#pragma once

#include "Graphics/GEntityItem.hpp"

namespace totcad {

class GLineEntity;

class GLineItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GLineItem(GLineEntity *entity);
    GLineEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GLineEntity *m_entity{nullptr};
};

} // namespace totcad
