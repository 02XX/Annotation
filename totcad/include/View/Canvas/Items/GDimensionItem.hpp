#pragma once

#include "View/Canvas/Items/GEntityItem.hpp"

namespace totcad {

class GDimensionEntity;

class GDimensionItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GDimensionItem(GDimensionEntity *entity);
    GDimensionEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GDimensionEntity *m_entity{nullptr};
};

} // namespace totcad
