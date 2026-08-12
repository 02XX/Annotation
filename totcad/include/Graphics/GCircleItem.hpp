#pragma once

#include "Graphics/GEntityItem.hpp"

namespace totcad {

class GCircleEntity;

class GCircleItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GCircleItem(GCircleEntity *entity);
    GCircleEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GCircleEntity *m_entity{nullptr};
};

} // namespace totcad
