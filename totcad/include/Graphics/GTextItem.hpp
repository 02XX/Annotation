#pragma once

#include "Graphics/GEntityItem.hpp"

namespace totcad {

class GTextEntity;

class GTextItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GTextItem(GTextEntity *entity);
    GTextEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GTextEntity *m_entity{nullptr};
};

} // namespace totcad
